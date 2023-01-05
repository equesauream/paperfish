#include <string>
#include <iostream>

#include "square.h"

namespace engine {

int rowNumber(Square s) {
    return bitToSquare(s).at(1) - '0';
}

char colNumber(Square s) {
    return bitToSquare(s).at(0);
}

// a8 = bit 0
// h8 = bit 7
// a1 = bit 56
// h8 = bit 63
// converts a bit to a square
std::string bitToSquare(U64 bit) {
    int pos = 0;
    while (bit != 0) {
        while (bit < (1ULL << 63)) {
            ++pos;
            bit = bit << 1;
        }
        ++pos;
        bit = bit << 1;
    }
    --pos;
    return std::string(1, char('h' - ((pos % 8)))) + std::to_string((pos / 8) + 1);
}

// converts a square (e.g. "e4") to a bit
U64 squareToBit(const std::string& s) {
    if (s.length() != 2) {
        std::cout << "incorrect string length for squareToBit(): given string " << s << '\n';
    }
    U64 bit = 0;
    int c = s.at(0) - 'a';
    int r = 8 - (s.at(1) - '0');
    bit = (1ULL << (8*r + c));
    return bit;
}

}