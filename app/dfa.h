#ifndef DFA_H
#define DFA_H

#include <string>
#include <map>
#include <set>
#include "alphabet.h"
#include "syntaxtree.h"


class DFA {  // Deterministic finite automaton
    Alphabet alphabet;
    std::set<std::string> states;
    std::string initialState;
    std::set<std::string> finalStates;
    std::map<std::string, std::map<char, std::string>> transitions;
    //         from                 by       to

public:
    explicit DFA(const Alphabet& alphabet);
    explicit DFA(const SyntaxTree& tree);

    DFA(const std::string& alphabet, const std::set<std::string>& states, const std::string& initialState,
        const std::set<std::string>& finalStates, const std::map<std::string, std::map<char, std::string>>& transitions); // For tests.

    size_t size() const;
    bool isEmpty() const;

    std::set<std::string> getStates() const;
    bool hasState(const std::string& name) const;
    bool createState(const std::string& name, bool isFinal = false); // Return false if such a state already existed in the automaton.

    std::string getInitialState() const;
    bool isInitial(const std::string& name) const;   // Check if state 'name' is initial state of the automaton.
    bool setInitial(const std::string& name);        // Make state 'name' initial state of the automaton. Return false if there is no such state in the automaton.

    std::set<std::string> getFinalStates() const;
    bool isFinal(const std::string& name) const;
    bool makeFinal(const std::string& name);         // Return false if state 'name' already was in the automaton.

    bool hasTrans(const std::string& from, char transSymbol) const;
    std::string getTrans(const std::string& from, char transSymbol) const;  // Name of the state to which you can go from the 'from' state by the 'transSymbol' symbol.
                                                                            // Throws an exception if there is no such state.
    bool setTrans(const std::string& from, char by, const std::string& to); // If "from" or "to" was not the state of the automaton or "by"
                                                                            // was not an alphabet symbol, the return false.
                                                                            // Create new or overwrites existing transition otherwise.

    Alphabet getAlphabet() const;
    std::string toString() const;

    bool operator==(const DFA& other) const;   // Check if the automaton specify the same language
    bool operator!=(const DFA& other) const;
};

#endif // DFA_H
