#include <string>
#include <iostream>
#include <vector>

#include "square.h"

namespace engine {
    
int get_bit_index(const Square s) { return __builtin_ctzll(s);}

int rowNumber(Square s) {
    return 8 - get_bit_index(s) / 8;
}

char colNumber(Square s) {
    return 'a' + get_bit_index(s) % 8;
}

// a8 = bit 0
// h8 = bit 7
// a1 = bit 56
// h8 = bit 63
// converts a bit to a square
std::string bitToSquare(U64 bit) {
    const int pos = 63 - get_bit_index(bit);
    return std::string(1, char('h' - ((pos % 8)))) + std::to_string((pos / 8) + 1);
}

// converts a square (e.g. "e4") to a bit
U64 squareToBit(const std::string& s) {
    if (s.length() != 2) {
        std::cout << "incorrect string length for squareToBit(): given string " << s << '\n';
    }
    int c = s.at(0) - 'a';
    int r = 8 - (s.at(1) - '0');
    return 1ULL << (8*r + c);
}

std::vector<Square> bbToSquares(U64 board) {
    std::vector<Square> bitSquares;
    while (board != 0) {
        bitSquares.push_back(1ULL << get_bit_index(board));
        board &= board - 1;
    }
    return bitSquares;
}

int countBits(U64 num) {
    int c = 0;
    while (num != 0) {
        c += num & 1ULL;
        num <<= 1;
    }
    return c;
}

}