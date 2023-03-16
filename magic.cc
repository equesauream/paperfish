#include "magic.h"
#include "square.h"
#include "piece.h"
#include "type.h"

namespace engine {

//                     BE CAREFUL                       //
//         YOU ARE ENTERING THE ZONE OF MAGIC           //
//               ENTER AT YOUR OWN RISK                 //


namespace magic {

// magic numbers pulled from Nathan Lo's Magnum Carl
// https://github.com/nathanlo99/magnum_carl/blob/ff191fdcf343a3a9ff0c671777b2a7fecb662f77/magic.hpp#L203

const U64 rookMagics[64] = {
    0x0080002080400010ULL, // a1 (5 bits)
    0x0040001000200040ULL, // b1 (4 bits)
    0x0080100020008008ULL, // c1 (5 bits)
    0x0080100008008004ULL, // d1 (5 bits)
    0x0080080080040002ULL, // e1 (5 bits)
    0x0100010002080400ULL, // f1 (5 bits)
    0x0080008001000200ULL, // g1 (4 bits)
    0x0100020080402100ULL, // h1 (6 bits)
    0x0000800020400080ULL, // a2 (4 bits)
    0x0000400020005000ULL, // b2 (4 bits)
    0x0000801000200080ULL, // c2 (4 bits)
    0x0000800800100080ULL, // d2 (4 bits)
    0x0000800400080080ULL, // e2 (4 bits)
    0x0000800200040080ULL, // f2 (4 bits)
    0x0000800100020080ULL, // g2 (4 bits)
    0x0000800040800100ULL, // h2 (4 bits)
    0x0000208000400080ULL, // a3 (4 bits)
    0x0000404000201000ULL, // b3 (4 bits)
    0x0000808010002000ULL, // c3 (4 bits)
    0x0000808008001000ULL, // d3 (4 bits)
    0x0000808004000800ULL, // e3 (4 bits)
    0x0000808002000400ULL, // f3 (4 bits)
    0x0000010100020004ULL, // g3 (4 bits)
    0x0000020000804401ULL, // h3 (5 bits)
    0x0000208080004000ULL, // a4 (4 bits)
    0x0000200040005000ULL, // b4 (4 bits)
    0x0000100080200080ULL, // c4 (4 bits)
    0x0000080080100080ULL, // d4 (4 bits)
    0x0000040080080080ULL, // e4 (4 bits)
    0x0000020080040080ULL, // f4 (4 bits)
    0x0000010080800200ULL, // g4 (4 bits)
    0x0000800080004100ULL, // h4 (4 bits)
    0x0000204000800080ULL, // a5 (4 bits)
    0x0000200040401000ULL, // b5 (4 bits)
    0x0000100080802000ULL, // c5 (4 bits)
    0x0000080080801000ULL, // d5 (4 bits)
    0x0000040080800800ULL, // e5 (4 bits)
    0x0000020080800400ULL, // f5 (4 bits)
    0x0000020001010004ULL, // g5 (4 bits)
    0x0000800040800100ULL, // h5 (4 bits)
    0x0000204000808000ULL, // a6 (4 bits)
    0x0000200040008080ULL, // b6 (4 bits)
    0x0000100020008080ULL, // c6 (4 bits)
    0x0000080010008080ULL, // d6 (4 bits)
    0x0000040008008080ULL, // e6 (4 bits)
    0x0000020004008080ULL, // f6 (4 bits)
    0x0000010002008080ULL, // g6 (4 bits)
    0x0000008100420004ULL, // h6 (5 bits)
    0x0000204000800080ULL, // a7 (4 bits)
    0x0000200040008080ULL, // b7 (4 bits)
    0x0000100020008080ULL, // c7 (4 bits)
    0x0000080010008080ULL, // d7 (4 bits)
    0x0000040008008080ULL, // e7 (4 bits)
    0x0000020004008080ULL, // f7 (4 bits)
    0x0000800100020080ULL, // g7 (4 bits)
    0x0000800041000080ULL, // h7 (4 bits)
    0x0000208001004011ULL, // a8 (6 bits)
    0x0001001080204001ULL, // b8 (6 bits)
    0x0000090040102001ULL, // c8 (6 bits)
    0x0000200500100009ULL, // d8 (6 bits)
    0x0002000420100802ULL, // e8 (6 bits)
    0x0001000804000201ULL, // f8 (5 bits)
    0x0001000082000441ULL, // g8 (6 bits)
    0x0000008021040042ULL, // h8 (6 bits)
};

const U64 bishopMagics[64] = {
    0x0020011001004080ULL, // a1 (6 bits)
    0x0004042802002000ULL, // b1 (6 bits)
    0x0004010202000000ULL, // c1 (4 bits)
    0x0004040080000000ULL, // d1 (3 bits)
    0x0001104000000000ULL, // e1 (3 bits)
    0x0000821040000000ULL, // f1 (4 bits)
    0x0000821010040000ULL, // g1 (5 bits)
    0x0000410090012000ULL, // h1 (6 bits)
    0x0000080801080210ULL, // a2 (6 bits)
    0x0000101002004840ULL, // b2 (6 bits)
    0x0000040102020000ULL, // c2 (4 bits)
    0x0000040400800000ULL, // d2 (3 bits)
    0x0000011040000000ULL, // e2 (3 bits)
    0x0000008210400000ULL, // f2 (4 bits)
    0x0000004208044000ULL, // g2 (5 bits)
    0x0000002404020800ULL, // h2 (5 bits)
    0x0010800820080080ULL, // a3 (6 bits)
    0x0011000802008400ULL, // b3 (6 bits)
    0x0002001000220020ULL, // c3 (5 bits)
    0x0000800802004000ULL, // d3 (4 bits)
    0x0000800400a00000ULL, // e3 (4 bits)
    0x0002000040500400ULL, // f3 (5 bits)
    0x0000800208040200ULL, // g3 (5 bits)
    0x0000800040480800ULL, // h3 (5 bits)
    0x0004400004080800ULL, // a4 (5 bits)
    0x0002200010010200ULL, // b4 (5 bits)
    0x0001100001004200ULL, // c4 (5 bits)
    0x0010040000401020ULL, // d4 (5 bits)
    0x0000840000802000ULL, // e4 (4 bits)
    0x000040800100a000ULL, // f4 (5 bits)
    0x0001020000421000ULL, // g4 (5 bits)
    0x0000404000840400ULL, // h4 (5 bits)
    0x0008084000080200ULL, // a5 (5 bits)
    0x0001082000020400ULL, // b5 (5 bits)
    0x0000440200100020ULL, // c5 (5 bits)
    0x0000020080080080ULL, // d5 (4 bits)
    0x0008020400001100ULL, // e5 (5 bits)
    0x0002004200010080ULL, // f5 (5 bits)
    0x0008008100008800ULL, // g5 (5 bits)
    0x0002008100002400ULL, // h5 (5 bits)
    0x0000842008002000ULL, // a6 (5 bits)
    0x0001080104001000ULL, // b6 (5 bits)
    0x0000082088001000ULL, // c6 (5 bits)
    0x0000002011000800ULL, // d6 (4 bits)
    0x0000080100400400ULL, // e6 (4 bits)
    0x0040008080800100ULL, // f6 (5 bits)
    0x0010010104000108ULL, // g6 (6 bits)
    0x0008008080840200ULL, // h6 (6 bits)
    0x0001010802400000ULL, // a7 (5 bits)
    0x0001010082200000ULL, // b7 (5 bits)
    0x0000002084100000ULL, // c7 (4 bits)
    0x0000000020880000ULL, // d7 (3 bits)
    0x0000001002020000ULL, // e7 (3 bits)
    0x0000040408020000ULL, // f7 (4 bits)
    0x0010050808004000ULL, // g7 (6 bits)
    0x0010040084004400ULL, // h7 (6 bits)
    0x0000104808041000ULL, // a8 (6 bits)
    0x0000002402021000ULL, // b8 (5 bits)
    0x0000000020841000ULL, // c8 (4 bits)
    0x0000000000208800ULL, // d8 (3 bits)
    0x0000000010020200ULL, // e8 (3 bits)
    0x0000000404080200ULL, // f8 (4 bits)
    0x0000100401080220ULL, // g8 (6 bits)
    0x0010020081020200ULL, // h8 (6 bits)
};

const int rookOccupancy[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const int bishopOccupancy[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

U64 rookMask[64];
U64 bishopMask[64];

// first index for piece, second index for square
// bishop and rook are found in separate table
U64 attacks[12][64];

U64 rookAttacks[64][4096] = {0};
U64 bishopAttacks[64][1024] = {0};

// s is 0-63
Square rookHash(int s, U64 bitmask) {
    return (bitmask * rookMagics[s]) >> (64 - rookOccupancy[s]);
}

// s is 0-63
Square bishopHash(int s, U64 bitmask) {
    return (bitmask * bishopMagics[s]) >> (64 - bishopOccupancy[s]);
}

U64 getRookAttacks(Square s, U64 blockers) {
    blockers &= ~s;
    Square cur = s;
    int index = getSquareIndex(s);
    U64 tmp = 0;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & rookMask[index]) != 0) &&
           colNumber(cur) > A) {
        tmp |= cur >> 1;
        cur >>= 1;
    }
    cur = s;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & rookMask[index]) != 0) &&
           rowNumber(cur) < 8) {
        tmp |= cur >> 8;
        cur >>= 8;
    }
    cur = s;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & rookMask[index]) != 0) &&
           colNumber(cur) < H) {
        tmp |= cur << 1;
        cur <<= 1;
    }
    cur = s;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & rookMask[index]) != 0) &&
           rowNumber(cur) > 1) {
        tmp |= cur << 8;
        cur <<= 8;
    }
    return tmp;
}

U64 getBishopAttacks(Square s, U64 blockers) {
    blockers &= ~s;
    Square cur = s;
    int index = getSquareIndex(s);
    U64 tmp = 0;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & bishopMask[index]) != 0) &&
           colNumber(cur) < H && rowNumber(cur) < 8) {
        tmp |= cur >> 7;
        cur >>= 7;
    }
    cur = s;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & bishopMask[index]) != 0) &&
           colNumber(cur) > A && rowNumber(cur) < 8) {
        tmp |= cur >> 9;
        cur >>= 9;
    }
    cur = s;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & bishopMask[index]) != 0) &&
           colNumber(cur) > A && rowNumber(cur) > 1) {
        tmp |= cur << 7;
        cur <<= 7;
    }
    cur = s;
    while ((cur & blockers) == 0 && 
           (cur == s || (cur & bishopMask[index]) != 0) &&
           colNumber(cur) < H && rowNumber(cur) > 1) {
        tmp |= cur << 9;
        cur <<= 9;
    }
    return tmp;
}

void initMagics() {
    initBishopTable();
    initRookTable();
    
    initPawnAttacks();
    initKnightAttacks();
    initKingAttacks();
    initBishopAttacks();
    initRookAttacks();
}

// returns the powerset of its set bits
std::vector<U64> generateBlockers(U64 bits) {
    std::vector<U64> result {0};
    
    for(int bit = 0; bit < 64; ++bit) {
        if(bits & (1ULL << bit)) {
            //iterate result, doubling its size
            int sz = result.size();
            result.reserve(result.size() * 2);
            for(int i = 0; i < sz; ++i) {
                result.push_back(result[i] | (1ULL << bit));
            }
        }
    }
    return result;
}

// apply the hash function to fill the attack hash table for rook
void initRookAttacks() {
    for (int index = 0; index < 64; ++index) {
        Square s = getSquare(index);
        for (const auto& blocker : generateBlockers(rookMask[index])) {
            Square hashVal = rookHash(index, blocker & rookMask[index]);
            if (rookAttacks[index][hashVal] != 0)
                continue;

            rookAttacks[index][hashVal] = getRookAttacks(s, blocker & rookMask[index]);
        }
    }
}

// apply the hash function to fill the attack hash table for rook
void initBishopAttacks() {
    for (int index = 0; index < 64; ++index) {
        Square s = getSquare(index);
        for (const auto& blocker : generateBlockers(bishopMask[index])) {
            Square hashVal = bishopHash(index, blocker & bishopMask[index]);
            if (bishopAttacks[index][hashVal] != 0)
                continue;
            
            bishopAttacks[index][hashVal] = getBishopAttacks(s, blocker & bishopMask[index]);
        }
    }
}

// initialize the rookmasks
void initRookTable() {
    for (int j = 0; j < 64; ++j) {
        Square pos = getSquare(j);
        Square tmp = 0;
        Col r = colNumber(pos);
        int c = rowNumber(pos);
        int i = 1;
        while (r > B) {
            --r;
            tmp |= pos >> i;
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < G) {
            ++r;
            tmp |= pos << i;
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (c > 2) {
            --c;
            tmp |= pos << i * 8;
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (c < 7) {
            ++c;
            tmp |= pos >> i * 8;
            ++i;
        }

        rookMask[getSquareIndex(pos)] = tmp;
    }
}

// initialize the bishopmasks
void initBishopTable() {
    for (int j = 0; j < 64; ++j) {
        Square pos = getSquare(j);
        Square tmp = 0;
        Col r = colNumber(pos);
        int c = rowNumber(pos);
        int i = 1;
        while (r > B && c > 2) {
            --r;
            --c;
            tmp |= pos << i * 7;
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r > B && c < 7) {
            --r;
            ++c;
            tmp |= pos >> i * 9;
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < G && c > 2) {
            ++r;
            --c;
            tmp |= pos << i * 9;
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < G && c < 7) {
            ++r;
            ++c;
            tmp |= pos >> i * 7;
            ++i;
        }
        
        bishopMask[getSquareIndex(pos)] = tmp;
    }
}

// initialize pawn attacks
void initPawnAttacks() {
    // white pawn
    for (int i = 0; i < 64; ++i) {
        Square pos = getSquare(i);
        Square tmp = 0;
        if (rowNumber(pos) == 2 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 16) |
                   (pos >> 7 ) |
                   (pos >> 9 );
        } else if (rowNumber(pos) == 2 && colNumber(pos) == A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 16) |
                   (pos >> 7 );
        } else if (rowNumber(pos) == 2 && colNumber(pos) == H) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 16) |
                   (pos >> 9 );
        } else if (rowNumber(pos) == 7 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 7 ) |
                   (pos >> 9 );
        } else if (rowNumber(pos) == 7 && colNumber(pos) == A) {
            tmp |= (pos >> 8 ) |
                   (pos >> 7 );
        } else if (rowNumber(pos) == 7 && colNumber(pos) == H) {
            tmp |= (pos >> 8 ) |
                   (pos >> 9 );
        } else if (colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 7 ) |
                   (pos >> 9 );
        } else if (colNumber(pos) == H) {
            tmp |= (pos >> 8 ) |
                   (pos >> 9 );
        } else if (colNumber(pos) == A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 7 );
        }
        attacks[whitePawn][getSquareIndex(pos)] = tmp;
    }

    // black pawn
    for (int i = 0; i < 64; ++i) {
        Square pos = getSquare(i);
        Square tmp = 0;
        if (rowNumber(pos) == 7 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos << 8 ) | 
                   (pos << 16) |
                   (pos << 7 ) |
                   (pos << 9 );
        } else if (rowNumber(pos) == 7 && colNumber(pos) == A) {
            tmp |= (pos << 8 ) | 
                   (pos << 16) |
                   (pos << 9 );
        } else if (rowNumber(pos) == 7 && colNumber(pos) == H) {
            tmp |= (pos << 8 ) | 
                   (pos << 16) |
                   (pos << 7 );
        } else if (rowNumber(pos) == 2 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos << 8 ) | 
                   (pos << 7 ) |
                   (pos << 9 );
        } else if (rowNumber(pos) == 2 && colNumber(pos) == A) {
            tmp |= (pos << 8 ) |
                   (pos << 9 );
        } else if (rowNumber(pos) == 2 && colNumber(pos) == H) {
            tmp |= (pos << 8 ) | 
                   (pos << 7 );
        } else if (colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos << 8 ) | 
                   (pos << 7 ) |
                   (pos << 9 );
        } else if (colNumber(pos) == H) {
            tmp |= (pos << 8 ) | 
                   (pos << 7 );
        } else if (colNumber(pos) == A) {
            tmp |= (pos << 8 ) | 
                   (pos << 9 );
        }
        attacks[blackPawn][getSquareIndex(pos)] = tmp;
    }
}

// initialize knight attacks
void initKnightAttacks() {
    for (int i = 0; i < 64; ++i) {
        Square pos = getSquare(i);
        Square tmp = 0;
        if (colNumber(pos) > A && rowNumber(pos) > 2)
            tmp |= pos << 15;

        if (colNumber(pos) > A && rowNumber(pos) < 7)
            tmp |= pos >> 17;

        if (colNumber(pos) > B && rowNumber(pos) > 1)
            tmp |= pos << 6;

        if (colNumber(pos) > B && rowNumber(pos) < 8)
            tmp |= pos >> 10;

        if (colNumber(pos) < H && rowNumber(pos) > 2)
            tmp |= pos << 17;

        if (colNumber(pos) < H && rowNumber(pos) < 7)
            tmp |= pos >> 15;

        if (colNumber(pos) < G && rowNumber(pos) > 1)
            tmp |= pos << 10;

        if (colNumber(pos) < G && rowNumber(pos) < 8)
            tmp |= pos >> 6;

        attacks[blackKnight][getSquareIndex(pos)] = tmp;
        attacks[whiteKnight][getSquareIndex(pos)] = tmp;
    }
}

// initialize king attacks
void initKingAttacks() {
    // white king
    for (Square pos = A8; pos != NoSquare; pos <<= 1) {
        Square tmp = 0;

        if (colNumber(pos) > A)
            tmp |= pos >> 1;
        
        if (colNumber(pos) < H)
            tmp |= pos << 1;
        
        if (rowNumber(pos) > 1)
            tmp |= pos << 8;
        
        if (rowNumber(pos) < 8)
            tmp |= pos >> 8;
        
        if (rowNumber(pos) > 1 && colNumber(pos) > A)
            tmp |= pos << 7;
        
        if (rowNumber(pos) > 1 && colNumber(pos) < H)
            tmp |= pos << 9;
        
        if (rowNumber(pos) < 8 && colNumber(pos) > A)
            tmp |= pos >> 9;
        
        if (rowNumber(pos) < 8 && colNumber(pos) < H)
            tmp |= pos >> 7;
        
        if (pos == E1)
            tmp |= G1 | C1;

        attacks[whiteKing][getSquareIndex(pos)] = tmp;
    }

    // black king
    for (Square pos = A8; pos != NoSquare; pos <<= 1) {
        Square tmp = 0;

        if (colNumber(pos) > A)
            tmp |= pos >> 1;
        
        if (colNumber(pos) < H)
            tmp |= pos << 1;
        
        if (rowNumber(pos) > 1)
            tmp |= pos << 8;
        
        if (rowNumber(pos) < 8)
            tmp |= pos >> 8;
        
        if (rowNumber(pos) > 1 && colNumber(pos) > A)
            tmp |= pos << 7;
        
        if (rowNumber(pos) > 1 && colNumber(pos) < H)
            tmp |= pos << 9;
        
        if (rowNumber(pos) < 8 && colNumber(pos) > A)
            tmp |= pos >> 9;
        
        if (rowNumber(pos) < 8 && colNumber(pos) < H)
            tmp |= pos >> 7;
        
        if (pos == E8)
            tmp |= G8 | C8; 

        attacks[blackKing][getSquareIndex(pos)] = tmp;
    }
}

// return the attack bitmap
U64 getAttacks(Piece p, Square s, U64 blockers) {
    if (isPawn(p) || isKnight(p) || isKing(p))
        return attacks[p][getSquareIndex(s)];

    if (isRook(p)) {
        int index = getSquareIndex(s);
        return rookAttacks[index][rookHash(index, blockers & rookMask[index])];
    }

    if (isBishop(p)) {
        int index = getSquareIndex(s);
        return bishopAttacks[index][bishopHash(index, blockers & bishopMask[index])];
    }

    if (isQueen(p))
        return getAttacks(whiteBishop, s, blockers) | getAttacks(whiteRook, s, blockers);

    std::cout << "invalid piece: given piece " << p << '\n';
    return 0;
}

U64 getChecks(Piece p, Square s, U64 blockers) {
    if (isKnight(p) | isBishop(p) | isRook(p) | isQueen(p))
        return getAttacks(p, s, blockers);

    if (isPawn(p)) {
        U64 checks = getAttacks(p, s, blockers);
        if (p == blackPawn) {
            return checks & ~(s << 8) & ~(s << 16);
        } else {
            return checks & ~(s >> 8) & ~(s >> 16);
        }
    }

    if (isKing(p)) {
        U64 checks = getAttacks(p, s, blockers);
        if (p == blackKing) {
            if (s == E8) return checks & ~G8 & ~C8;
            else return checks;
        } else {
            if (s == E1) return checks & ~G1 & ~C1;
            else return checks;
        }
    }
    return 0;
}

// print the bitboard
void printBitboard(U64 b) {
    for (int i = 0; i < 64; ++i) {
        if (getSquare(i) & b)
            std::cout << 1;
        else
            std::cout << 0;
        
        if (i % 8 == 7)
            std::cout << '\n';
    }
}

}

}
