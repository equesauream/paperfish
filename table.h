#ifndef TABLE_H
#define TABLE_H

#include <unordered_map>

#include "move.h"
#include "position.h"

using Key = unsigned long long;

namespace engine {

struct TTInfo {
    Move bestMove;
    int eval;
    int depth;
};

void initZobristTables();

struct ZHash {
    Key operator()(const Position& p) const;
};

TTInfo& at(const Position& p);

}

#endif
