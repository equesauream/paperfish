#include <unordered_map>
#include <iostream>
#include <vector>

#include "magic.h"
#include "square.h"
#include "piece.h"

namespace engine {

//                     BE CAREFUL                       //
//         YOU ARE ENTERING THE ZONE OF MAGIC           //
//               ENTER AT YOUR OWN RISK                 //


namespace magic {

// magic numbers pulled from GunshipPenguin's Shallow Blue:
// https://github.com/GunshipPenguin/shallow-blue/blob/c6d7e9615514a86533a9e0ffddfc96e058fc9cfd/src/attacks.h#L120

const U64 rookMagics[64] = {
    0xa8002c000108020ULL,  0x6c00049b0002001ULL,  0x100200010090040ULL,  0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL,  0x280020001001080ULL,  0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL,  0x402800800040080ULL,  0xb000401004208ULL,    0x2409000100040200ULL,
    0x1002100004082ULL,    0x22878001e24000ULL,   0x1090810021004010ULL, 0x801030040200012ULL,  0x500808008001000ULL,
    0xa08018014000880ULL,  0x8000808004000200ULL, 0x201008080010200ULL,  0x801020000441091ULL,  0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL,     0xd14880480100080ULL,  0x12040280080080ULL,   0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL,  0x491604001800080ULL,  0x100401000402001ULL,  0x4820010021001040ULL,
    0x400402202000812ULL,  0x209009005000802ULL,  0x810800601800400ULL,  0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL,   0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL,   0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL,  0xe0100040002240ULL,
    0x200100401700ULL,     0x2244100408008080ULL, 0x8000400801980ULL,    0x2000810040200ULL,    0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL,   0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL,    0x4000011008020084ULL, 0x26002114058042ULL
};

const U64 bishopMagics[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL,  0xc00444222012000aULL, 0x28808801216001ULL,   0x400492088408100ULL,  0x201c401040c0084ULL,
    0x840800910a0010ULL,   0x82080240060ULL,      0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL,  0x4800201208ca00ULL,   0xf18140408012008ULL,  0x1004002802102001ULL,
    0x841000820080811ULL,  0x40200200a42008ULL,   0x800054042000ULL,     0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL,    0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL,  0x4800404002011c00ULL, 0x104442040404200ULL,  0x511080202091021ULL,  0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL,  0x102008e00040242ULL,  0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL,  0x19001802081400ULL,   0x200014208040080ULL,  0x3308082008200100ULL,
    0x41010500040c020ULL,  0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL,  0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL,      0x2410204010040ULL,    0x40100400809000ULL,
    0x822088220820214ULL,  0x40808090012004ULL,   0x910224040218c9ULL,   0x402814422015008ULL,  0x90014004842410ULL,
    0x1000042304105ULL,    0x10008830412a00ULL,   0x2520081090008908ULL, 0x40102000a0a60140ULL,
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

U64 rookAttacks[64][4096];
U64 bishopAttacks[64][1024];

// s is 0-63
Square rookHash(int s, U64 bitmask) {
    return ((bitmask * rookMagics[s]) << (64 - rookOccupancy[s])) >> (64 - rookOccupancy[s]);
}

// s is 0-63
Square bishopHash(int s, U64 bitmask) {
    return ((bitmask * bishopMagics[s]) << (64 - bishopOccupancy[s])) >> (64 - bishopOccupancy[s]);
}

U64 getRookAttacks(Square s, U64 blockers) {
    Square cur = s;
    U64 tmp;
    while ((cur & blockers) != 0) {
        tmp |= cur >> 1;
        cur >>= 1;
    }
    cur = s;
    while ((cur & blockers) != 0) {
        tmp |= cur >> 8;
        cur >>= 8;
    }
    cur = s;
    while ((cur & blockers) != 0) {
        tmp |= cur << 1;
        cur <<= 1;
    }
    cur = s;
    while ((cur & blockers) != 0) {
        tmp |= cur << 8;
        cur <<= 8;
    }
    return tmp;
}

U64 getBishopAttacks(Square s, U64 blockers) {
    Square cur = s;
    U64 tmp;
    while ((cur & blockers) != 0) {
        tmp |= cur >> 7;
        cur >>= 7;
    }
    cur = s;
    while ((cur & blockers) != 0) {
        tmp |= cur >> 9;
        cur >>= 9;
    }
    cur = s;
    while ((cur & blockers) != 0) {
        tmp |= cur << 7;
        cur <<= 7;
    }
    cur = s;
    while ((cur & blockers) != 0) {
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
            for(int i = 0; i < sz; ++i) {
                result.push_back(result[i] | (1 << bit));
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
            if (rookAttacks[s][hashVal] != 0)
                continue;

            rookAttacks[s][hashVal] = getRookAttacks(index, blocker & rookMask[index]);
        }
    }
}

// apply the hash function to fill the attack hash table for rook
void initBishopAttacks() {
    for (int index = 0; index < 64; ++index) {
        Square s = getSquare(index);
        for (const auto& blocker : generateBlockers(bishopMask[index])) {
            Square hashVal = bishopHash(index, blocker & bishopMask[index]);
            if (bishopAttacks[s][hashVal] != 0)
                continue;

            bishopAttacks[s][hashVal] = getBishopAttacks(index, blocker & bishopMask[index]);
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
    for (int i = 8; i < 56; ++i) {
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
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 7 ) |
                   (pos >> 9 );
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == H) {
            tmp |= (pos >> 8 ) |
                   (pos >> 9 );
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == A) {
            tmp |= (pos >> 8 ) | 
                   (pos >> 7 );
        }
        attacks[whitePawn][getSquareIndex(pos)] = tmp;
    }

    // black pawn
    for (int i = 8; i < 56; ++i) {
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
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) < H && colNumber(pos) > A) {
            tmp |= (pos << 8 ) | 
                   (pos << 7 ) |
                   (pos << 9 );
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == H) {
            tmp |= (pos << 8 ) | 
                   (pos << 7 );
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == A) {
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
