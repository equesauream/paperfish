#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <vector>
#include <map>

#include "square.h"
#include "piece.h"
class Position;

typedef unsigned long long U64;
using Square = unsigned long long;
using Piece = int;

namespace engine {

struct Move {
    SquareIndex source;
    SquareIndex dest;
    Piece piece;

    // bitmask of 4 bits e.g. 1101 for KQq
    uint8_t castleRights; // true for enabled
    // enabled if a pawn takes a double step, NoSquare if not
    SquareIndex enPassantSquare;
    // defaults if no promotion piece is '-'
    Piece promotionPiece = NoPiece;
    
  public:
    Move();
    Move(Square s, Square d, Piece piece, Piece prom = NoPiece);
};

bool operator==(const Move& lhs, const Move& rhs);
std::ostream& operator<<(std::ostream& out, const Move& m);

}

#endif
