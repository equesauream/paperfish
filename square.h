#ifndef SQUARE_H
#define SQUARE_H

typedef unsigned long long U64;

using Square = unsigned long long;

namespace engine {

enum {
    NoSquare = 0,

    A8 = 1ULL << 0,
    B8 = 1ULL << 1,
    C8 = 1ULL << 2,
    D8 = 1ULL << 3,
    E8 = 1ULL << 4,
    F8 = 1ULL << 5,
    G8 = 1ULL << 6,
    H8 = 1ULL << 7,
    
    A7 = 1ULL << 8,
    B7 = 1ULL << 9,
    C7 = 1ULL << 10,
    D7 = 1ULL << 11,
    E7 = 1ULL << 12,
    F7 = 1ULL << 13,
    G7 = 1ULL << 14,
    H7 = 1ULL << 15,
    
    A6 = 1ULL << 16,
    B6 = 1ULL << 17,
    C6 = 1ULL << 18,
    D6 = 1ULL << 19,
    E6 = 1ULL << 20,
    F6 = 1ULL << 21,
    G6 = 1ULL << 22,
    H6 = 1ULL << 23,
    
    A5 = 1ULL << 24,
    B5 = 1ULL << 25,
    C5 = 1ULL << 26,
    D5 = 1ULL << 27,
    E5 = 1ULL << 28,
    F5 = 1ULL << 29,
    G5 = 1ULL << 30,
    H5 = 1ULL << 31,
    
    A4 = 1ULL << 32,
    B4 = 1ULL << 33,
    C4 = 1ULL << 34,
    D4 = 1ULL << 35,
    E4 = 1ULL << 36,
    F4 = 1ULL << 37,
    G4 = 1ULL << 38,
    H4 = 1ULL << 39,
    
    A3 = 1ULL << 40,
    B3 = 1ULL << 41,
    C3 = 1ULL << 42,
    D3 = 1ULL << 43,
    E3 = 1ULL << 44,
    F3 = 1ULL << 45,
    G3 = 1ULL << 46,
    H3 = 1ULL << 47,
    
    A2 = 1ULL << 48,
    B2 = 1ULL << 49,
    C2 = 1ULL << 50,
    D2 = 1ULL << 51,
    E2 = 1ULL << 52,
    F2 = 1ULL << 53,
    G2 = 1ULL << 54,
    H2 = 1ULL << 55,
    
    A1 = 1ULL << 56,
    B1 = 1ULL << 57,
    C1 = 1ULL << 58,
    D1 = 1ULL << 59,
    E1 = 1ULL << 60,
    F1 = 1ULL << 61,
    G1 = 1ULL << 62,
    H1 = 1ULL << 63,
};

// returns the row number of a square e.g. rowNumber(E7) == 7
// O(1)
int rowNumber(Square s);

// returns the column number of a square e.g. colNumber(E7) == 'E'
// O(1)
char colNumber(Square s);

// converts a bit (Square) to its string
// O(1)
std::string bitToSquare(U64 bit);

// opposite of above
// O(1)
U64 squareToBit(const std::string& s);

// returns a vector of all set squares of a bitboard
// O(1)
std::vector<Square> bbToSquares(U64 board);

// returns the number of set bits
int countBits(U64 num);

}

#endif
