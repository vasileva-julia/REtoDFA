#ifndef ALPHABET_H
#define ALPHABET_H

#include <string>
#include <set>


class Alphabet {   // Contains alphabet symbols (chars)
    std::set<char> charset;
public:
    Alphabet() = default;

    explicit Alphabet(const std::string& rawString);
    explicit Alphabet(std::string&& rawString);
    explicit Alphabet(const std::set<char>& charset);
    explicit Alphabet(std::set<char>&& charset);
    explicit Alphabet(const char* rawString);

    std::string toString() const;

    bool hasChar(char c) const;   // Check if the alphabet contains symbol c.
    size_t size() const;          // Number of symbols in the alphabet.
    bool insert(char c);          // Add symbol c to the alphabet.

    bool operator==(const Alphabet &other) const;
    bool operator!=(const Alphabet &other) const;

    std::set<char>::const_iterator begin() const;
    std::set<char>::const_iterator end() const;
};

#endif // ALPHABET_H
