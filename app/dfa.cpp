#include "dfa.h"

#include <queue>
#include <algorithm>


DFA::DFA(const Alphabet &alphabet) : alphabet(alphabet) {
    createState("0", false);
    setInitial("0");
}

DFA::DFA(const SyntaxTree &tree) : DFA(tree.getAlphabet()) {

    std::map<std::set<size_t>, std::string> setsToStates;
    std::map<std::string, std::set<size_t>> statesToSets;

    setsToStates[tree.getRootFirstpos()] = initialState;
    statesToSets[initialState] = tree.getRootFirstpos();
    size_t lpos = tree.getLastPosition();

    if (tree.getRootFirstpos().find(lpos) != tree.getRootFirstpos().end())
        makeFinal(initialState);

    std::queue<std::string> unprocessedDfaStates;
    unprocessedDfaStates.push(initialState);
    int stateNum = 1;
    while (!unprocessedDfaStates.empty()) {
        std::string curState = unprocessedDfaStates.front();
        unprocessedDfaStates.pop();
        const std::set<size_t>& curStateSet = statesToSets[curState];

        for (char symbol : alphabet) {
            if (tree.hasSymbol(symbol)) {
                std::set<size_t> newStateSet;

                for (size_t position : tree.allPosForSymb(symbol)) {
                    if (curStateSet.find(position) != curStateSet.end()) {
                        const std::set<size_t>& followpos = tree.getFollowpos(position);
                        newStateSet.insert(followpos.begin(), followpos.end());
                    }
                }

                if (!newStateSet.empty()) {
                    auto it = setsToStates.find(newStateSet);
                    if (it == setsToStates.end()) {
                        std::string newStateName = std::to_string(stateNum);
                        ++stateNum;
                        createState(newStateName, false);
                        setsToStates[newStateSet] = newStateName;
                        statesToSets[newStateName] = newStateSet;
                        if (newStateSet.find(lpos) != newStateSet.end())
                            makeFinal(newStateName);
                        unprocessedDfaStates.push(newStateName);
                        setTrans(curState, symbol, newStateName);
                    } else {
                        setTrans(curState, symbol, it->second);
                    }
                }
            }
        }
    }
}

DFA::DFA(const std::string& alphabet, const std::set<std::string>& states, const std::string& initialState,
    const std::set<std::string>& finalStates, const std::map<std::string, std::map<char, std::string>>& transitions) :
    alphabet(alphabet), states(states), initialState(initialState), finalStates(finalStates), transitions(transitions) {}

size_t DFA::size() const {
    return states.size();
}

bool DFA::isEmpty() const {
    return (size() == 1) && (finalStates.size() == 0);
}

std::set<std::string> DFA::getStates() const {
    return states;
}

bool DFA::hasState(const std::string &name) const {
    return states.find(name) != states.end();
}

bool DFA::createState(const std::string &name, bool isFinal) {
    auto [it, wasInserted] = states.insert(name);
    if (isFinal)
        finalStates.insert(name);
    else
        finalStates.erase(name);
    return wasInserted;
}


std::string DFA::getInitialState() const {
    return initialState;
}

bool DFA::isInitial(const std::string &name) const {
    return initialState == name;
}

bool DFA::setInitial(const std::string &name) {
    if (states.find(name) == states.end())
        return false;
    initialState = name;
    return true;
}

std::set<std::string> DFA::getFinalStates() const {
    return finalStates;
}

bool DFA::isFinal(const std::string &name) const {
    return finalStates.find(name) != finalStates.end();
}

bool DFA::makeFinal(const std::string &name) {
    return finalStates.insert(name).second;
}

bool DFA::hasTrans(const std::string &from, char transSymbol) const {
    auto it = transitions.find(from);
    if (it == transitions.end())
        return false;
    return it->second.find(transSymbol) != it->second.end();
}

std::string DFA::getTrans(const std::string &from, char transSymbol) const {
    return transitions.at(from).at(transSymbol);
}

bool DFA::setTrans(const std::string &from, char by, const std::string &to) {
    if (states.find(from) == states.end() ||
            states.find(to) == states.end() ||
            !alphabet.hasChar(by))
        return false;
    transitions[from][by] = to;
    return true;
}

Alphabet DFA::getAlphabet() const {
    return alphabet;
}

std::string DFA::toString() const {
    std::string result;

    result += alphabet.toString() + '\n';

    for (const std::string& state : states) {
        if (isFinal(state))
            result += "\n[[" + state + "]]";
        else
            result += "\n[" + state + "]";
        if (state == initialState)
            result += "<-";
    }

    for (const auto& [state, stateTransitions] : transitions) {
        std::string stateName;
        if (isFinal(state))
            stateName += "\n[[" + state + "]]";
        else
            stateName += "\n[" + state + "]";
        for (const auto& [by, toState] : stateTransitions) {
            result += stateName + " ";
            result.push_back(by);
            if (isFinal(toState))
                result += " [[" + toState + "]]";
            else
                result += " [" + toState + "]";
        }
    }

    return result;
}

bool DFA::operator==(const DFA& other) const {
    if (this == &other)
        return true;
    if (alphabet != other.alphabet)
        return false;
    const std::string nonExistentState = "";
    std::set<std::pair<std::string, std::string>> usedStates = {{nonExistentState, nonExistentState}};
    std::queue<std::pair<std::string, std::string>> statesQueue;
    statesQueue.push({getInitialState(), other.getInitialState()});

    while (!statesQueue.empty()) {

        auto [state, otherState] = statesQueue.front();
        statesQueue.pop();

        if (isFinal(state) != other.isFinal(otherState))
            return false;

        usedStates.insert({state, otherState});

        auto transIt = transitions.find(state);
        auto otherTransIt = other.transitions.find(otherState);

        std::pair<std::string, std::string> nextStates = std::make_pair(
            (transIt == transitions.end() ? nonExistentState : state),
            (otherTransIt == other.transitions.end() ? nonExistentState : otherState)
        );

        for (auto symbol : alphabet) {
            if (nextStates.first != nonExistentState) {
                auto it = transIt->second.find(symbol);
                nextStates.first = (it == transIt->second.end() ? nonExistentState : it->second);
            }
            if (nextStates.second != nonExistentState) {
                auto otherIt = otherTransIt->second.find(symbol);
                nextStates.second = (otherIt == otherTransIt->second.end() ? nonExistentState : otherIt->second);
            }

            if (usedStates.find(nextStates) == usedStates.end()) {
                statesQueue.push(nextStates);
            }
        }
    }
    return true;
}

bool DFA::operator!=(const DFA& other) const {
    return !(*this == other);
}
