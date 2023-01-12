#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <vector>
#include <map>

#include "square.h"
class Position;

typedef unsigned long long U64;
using Square = unsigned long long;

namespace engine {

struct Move {
    Square source;
    Square dest;
    char piece;

    // bitmask of 4 bits e.g. 1101 for KQq
    int castleRights; // true for enabled
    // enabled if a pawn takes a double step, NoSquare if not
    Square enPassantSquare;
    // defaults if no promotion piece is '-'
    char promotionPiece = '-';
    
  public:
    Move();
    Move(Square s, Square d, char piece, char prom = '-');
};

bool operator==(const Move& lhs, const Move& rhs);
std::ostream& operator<<(std::ostream& out, const Move& m);

}

#endif
