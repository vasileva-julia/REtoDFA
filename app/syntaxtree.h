#ifndef S_TREE_H
#define S_TREE_H

#include <set>
#include <map>
#include <vector>
#include <stack>
#include <memory>
#include "alphabet.h"


class SyntaxTree {   // Сontains useful information about the extended regular expression syntax tree
                     //    regular_expression -> (regular_expression)#  == extended_regular_expression
    struct Node {
        bool nullable = false;
        std::set<size_t> firstpos;
        std::set<size_t> lastpos;
    };

    std::set<size_t> rootFirstpos;
    std::map<size_t, std::set<size_t>> followpos;
    std::map<char, std::set<size_t>> symbToPos;       // Symbol -> it's positions in the string (extended regex)

public:
    explicit SyntaxTree(const std::string& regex);

    bool hasSymbol(char symb) const;
    const std::set<size_t>& allPosForSymb(char symb) const; // Throw exception if hasSymbol(symb) == false. The numbering of positions starts from 1.

    size_t getLastPosition() const; // Position of special symbol #.

    Alphabet getAlphabet() const;   // Alphabet of string (regex) characters

    bool hasPos(size_t position) const;                           // Check if position is in the string. The numbering of positions starts from 1.
    const std::set<size_t>& getRootFirstpos() const;              // Firstpos of the root of the syntax tree.
    const std::set<size_t>& getFollowpos(size_t position) const;  // Followpos of position 'position'. Throw exception if has_pos(postiton) == false.

private:
    std::unique_ptr<Node> processSymbol(char symbol, size_t& symbPosition);
    void processOperation(char symbol, std::stack<std::unique_ptr<SyntaxTree::Node>>& nodeSt);
};

#endif // S_TREE_H
