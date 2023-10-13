#include "syntaxtree.h"

#include <vector>
#include <stack>
#include <algorithm>
#include <set>
#include <iostream>
#include <memory>
#include <set>


enum symb : char {
    EPS = '.',
    OR = '|',
    CAT = '+',
    STAR = '*',
    OPEN_B = '(',
    CLOSE_B = ')'
};

static bool implicitConcatenation(char prevSymb, char curSymb);
static bool implicitEpsilon(char prevSymb, char curSymb);
static bool isSymb(char c);
static bool inOrder(char a, char b);


SyntaxTree::SyntaxTree(const std::string& regex) {

    std::string regexS;
    if (regex.size() == 0) {
        regexS = "#";
    } else {
        regexS = "(" + regex + ")#";
    }

    std::stack<char> st;
    std::stack<std::unique_ptr<Node>> nodeSt;
    char prevSymb = OPEN_B;
    size_t curPosition = 1;

    for (char curSymb : regexS) {
        if (std::isblank(curSymb))
            continue;

        if (implicitConcatenation(prevSymb, curSymb)) { // add implicit concatenation
            while (!st.empty() && st.top() != OPEN_B && inOrder(st.top(), CAT)) {
                processOperation(st.top(), nodeSt);
                st.pop();
            }
            st.push(CAT);
        } else if (implicitEpsilon(prevSymb, curSymb)) { // add implicit EPS
            nodeSt.push(processSymbol(EPS, curPosition));
        }

        if (curSymb == OPEN_B) {
            st.push(curSymb);
        } else if (curSymb == CLOSE_B) {
            while (!st.empty() && st.top() != OPEN_B) {
                processOperation(st.top(), nodeSt);
                st.pop();
            }
            if (!st.empty())
                st.pop();
        } else if (curSymb == STAR || curSymb == OR || curSymb == CAT) {
            while (!st.empty() && st.top() != OPEN_B && inOrder(st.top(), curSymb)) {
                processOperation(st.top(), nodeSt);
                st.pop();
            }
            st.push(curSymb);
        } else {
            nodeSt.push(processSymbol(curSymb, curPosition));
        }

        prevSymb = curSymb;
    }

    while (!st.empty()) {
        char curSymb = st.top();
        st.pop();
        processOperation(curSymb, nodeSt);
    }

    rootFirstpos = std::move(nodeSt.top()->firstpos);
}

std::unique_ptr<SyntaxTree::Node> SyntaxTree::processSymbol(char symbol, size_t& symbPosition) {
    auto newNode = std::make_unique<Node>();
    if (symbol == EPS) {
        newNode->nullable = true;
    } else {
        newNode->firstpos.insert(symbPosition);
        newNode->lastpos.insert(symbPosition);
        symbToPos[symbol].insert(symbPosition);
        followpos[symbPosition] = {};
        ++symbPosition;
    }
    return newNode;
}

void SyntaxTree::processOperation(char symbol, std::stack<std::unique_ptr<SyntaxTree::Node>>& nodeSt) {
    auto newNode = std::make_unique<Node>();

    auto b = std::move(nodeSt.top());
    nodeSt.pop();
    if (symbol == STAR) {
        newNode->nullable = true;
        newNode->firstpos = b->firstpos;
        newNode->lastpos = b->lastpos;
        for (auto i : b->lastpos) {
            followpos[i].insert(b->firstpos.begin(), b->firstpos.end());
        }
        nodeSt.push(std::move(newNode));
        return;
    }

    auto a = std::move(nodeSt.top());
    nodeSt.pop();
    if (symbol == OR) {
        newNode->nullable = a->nullable || b->nullable;
        std::set_union(a->firstpos.begin(), a->firstpos.end(), b->firstpos.begin(),
                b->firstpos.end(), std::inserter(newNode->firstpos, newNode->firstpos.begin()));
        std::set_union(a->lastpos.begin(), a->lastpos.end(), b->lastpos.begin(),
                b->lastpos.end(), std::inserter(newNode->lastpos, newNode->lastpos.begin()));
    } else if (symbol == CAT) {
        newNode->nullable = a->nullable && b->nullable;
        if (a->nullable) {
            std::set_union(a->firstpos.begin(), a->firstpos.end(), b->firstpos.begin(),
                    b->firstpos.end(), std::inserter(newNode->firstpos, newNode->firstpos.begin()));
        } else {
            newNode->firstpos = a->firstpos;
        }
        if (b->nullable) {
            std::set_union(a->lastpos.begin(), a->lastpos.end(), b->lastpos.begin(),
                    b->lastpos.end(), std::inserter(newNode->lastpos, newNode->lastpos.begin()));
        } else {
            newNode->lastpos = b->lastpos;
        }

        for (auto i : a->lastpos) {
            followpos[i].insert(b->firstpos.begin(), b->firstpos.end());
        }
    }
    nodeSt.push(std::move(newNode));
}

bool SyntaxTree::hasSymbol(char symb) const {
    return symbToPos.find(symb) != symbToPos.end();
}

const std::set<size_t>& SyntaxTree::allPosForSymb(char symb) const {
    return symbToPos.at(symb);
}

size_t SyntaxTree::getLastPosition() const {
    return *allPosForSymb('#').begin();
}

Alphabet SyntaxTree::getAlphabet() const {
    Alphabet alphabet;
    for (const auto& [symbol, _] : symbToPos)
        if (symbol != '#')
            alphabet.insert(symbol);
    return alphabet;
}

bool SyntaxTree::hasPos(size_t position) const {
    return followpos.find(position) != followpos.end();
}

const std::set<size_t>& SyntaxTree::getRootFirstpos() const {
    return rootFirstpos;
}

const std::set<size_t>& SyntaxTree::getFollowpos(size_t position) const {
    return followpos.at(position);
}


bool implicitConcatenation(char prevSymb, char curSymb) {
    return (prevSymb == CLOSE_B && (curSymb == OPEN_B || isSymb(curSymb)))
            || ((isSymb(prevSymb) || prevSymb == STAR)
                && (isSymb(curSymb) || curSymb == OPEN_B));
}

bool implicitEpsilon(char prevSymb, char curSymb) {
    return (((prevSymb == CAT || prevSymb == OR)
              && (curSymb == CLOSE_B || curSymb == OR || curSymb == CAT || curSymb == STAR))
            || (prevSymb == OPEN_B && (curSymb == OR || curSymb == CAT || curSymb == STAR || curSymb == CLOSE_B)));
}

bool isSymb(char c) {
    return c != OPEN_B && c != CLOSE_B && c != OR && c != CAT && c != STAR;
}

bool inOrder(char a, char b) { // a >= b
  switch (a) {
    case STAR:
        return true;
    case CAT:
        return b != STAR;
    case OR:
        return b == OR;
    default:
        return false;
  }
}
