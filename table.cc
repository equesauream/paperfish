#include <random>
#include "table.h"

namespace engine {

namespace Zobrist {

    Key table[12][64];
    Key enPassant[64];
    Key castling[0b1111];
    Key blackToMove;
}

void initZobristTables() {
    std::random_device rd;
    // mersenne twister
    std::mt19937 gen(rd());
    std::uniform_int_distribution<U64> distrib(0, (1ULL << 63));

    for (int i = 0; i <= 11; ++i) {
        for (int j = 0; i < 64; ++j) {
            Zobrist::table[i][j] = distrib(gen);
            Zobrist::enPassant[j] = distrib(gen);
        }
    }
    for (int i = 0; i <= 0b1111; ++i) {
        Zobrist::castling[i] = distrib(gen);
    }
    Zobrist::blackToMove = distrib(gen);
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
        h ^= Zobrist::enPassant[get_bit_index(p.enPassant)];
    return h;
}

std::unordered_map<Position, TTInfo, ZHash> transTable;

TTInfo& at(const Position& p) {
    if (transTable.count(p))
        return transTable.at(p);
    else {
        transTable[p] = TTInfo();
        return transTable.at(p);
    }
}

}
