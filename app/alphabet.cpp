#include "alphabet.h"

#include <string.h>


Alphabet::Alphabet(const std::string& rawString) {
    charset.insert(rawString.begin(), rawString.end());
}

Alphabet::Alphabet(std::string&& rawString) {
    charset.insert(std::make_move_iterator(rawString.begin()),
                   std::make_move_iterator(rawString.end()));
}

Alphabet::Alphabet(const std::set<char>& charset) : charset(charset) {}
Alphabet::Alphabet(std::set<char>&& charset) : charset(std::move(charset)) {}

Alphabet::Alphabet(const char *rawString) {
    for (size_t i = 0; i < strlen(rawString); ++i)
        charset.insert(rawString[i]);
}

std::string Alphabet::toString() const {
    std::string res(charset.begin(), charset.end());
    return res;
}

bool Alphabet::hasChar(char c) const {
    return charset.find(c) != charset.end();
}

size_t Alphabet::size() const {
    return charset.size();
}

bool Alphabet::insert(char c) {
    return charset.insert(c).second;
}

bool Alphabet::operator==(const Alphabet &other) const {
    if (this == &other)
        return true;
    return std::equal(charset.begin(), charset.end(), other.begin(), other.end());
}

bool Alphabet::operator!=(const Alphabet &other) const {
    return !(*this == other);
}

std::set<char>::const_iterator Alphabet::begin() const {
    return charset.cbegin();
}

std::set<char>::const_iterator Alphabet::end() const {
    return charset.cend();
}
