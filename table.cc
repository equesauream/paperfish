#include "table.h"

namespace engine {

namespace Zobrist {
    Key table[12][64];
    Key enPassant[64];
    Key castling[0b1111 + 1];
    Key blackToMove;
}


std::random_device rd;
// mersenne twister
std::mt19937 gen(rd());

void initZobristTables() {
    for (int i = 0; i <= 11; ++i)
        for (int j = 0; j < 64; ++j)
            Zobrist::table[i][j] = randomKey();
    
    for (int i = 0; i < 64; ++i)
        Zobrist::enPassant[i] = randomKey();
    
    for (int i = 0; i < 15; ++i) 
        Zobrist::castling[i] = randomKey();

    Zobrist::blackToMove = randomKey();
}

Key ZHash::operator()(const Position& p) const {
    Key h = 0;
    if (p.turn == 1) {
        h ^= Zobrist::blackToMove;
    }
    for (int i = 0; i <= 11; ++i) {
        for (int j = 0; j <= 63; ++j) {
            if (p.thePosition.at(i) & (1ULL << j)) {
                h ^= Zobrist::table[i][j];
            }
        }
    }
    if (p.castleRights != 0)
        h ^= Zobrist::castling[p.castleRights];
    if (p.enPassant != NoSquare)
        h ^= Zobrist::enPassant[getSquareIndex(p.enPassant)];
    return h;
}

TTInfo::TTInfo() {}

TTInfo::TTInfo(Move m, int e, int d) :
    bestMove{m},
    eval{e},
    depth{d} {}

TTInfo& at(const Position& p) {
    if (transTable.count(p))
        return transTable.at(p);
    else {
        transTable[p] = TTInfo();
        return transTable.at(p);
    }
}

std::unordered_map<Position, TTInfo, ZHash> transTable;


unsigned long long rando(unsigned long long n) {
    std::uniform_int_distribution<unsigned long long> distrib(0, n);
    return distrib(gen);
}

Key randomKey() {
    return rando(UINT64_MAX);
}

}
