#ifndef TABLE_H
#define TABLE_H

#include "move.h"
#include "position.h"
#include "type.h"

namespace engine {

namespace Zobrist {
    extern Key table[12][64];
    extern Key enPassant[64];
    extern Key castling[0b1111 + 1];
    extern Key blackToMove;
}

struct TTInfo {
    Move bestMove;
    int eval;
    int depth;

    TTInfo();
    TTInfo(Move m, int e, int d);
};

void initZobristTables();

struct ZHash {
    Key operator()(const Position& p) const;
};

TTInfo& at(const Position& p);

extern std::unordered_map<Position, TTInfo, ZHash> transTable;

unsigned long long rando(unsigned long long n);

Key randomKey();

}

#endif
