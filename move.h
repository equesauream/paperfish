#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"
#include "type.h"
class Position;

namespace engine {

enum {
    Basic,
    Capture,
    LongPawn,
    EnPassant,
    ShortCastle,
    LongCastle
};

struct Move {
    MoveType type;
    SquareIndex source;
    SquareIndex dest;
    Piece piece;

    // bitmask of 4 bits e.g. 1101 for KQq
    uint8_t castleRights; // true for enabled
    // enabled if a pawn takes a double step, NoSquare if not
    SquareIndex enPassantSquare;
    // defaults if no promotion piece is NoPiece
    Piece promotionPiece = NoPiece;
    
  public:
    Move();
    Move(Square s, Square d, Piece piece, MoveType mt, Piece prom = NoPiece);
};

bool operator==(const Move& lhs, const Move& rhs);
std::ostream& operator<<(std::ostream& out, const Move& m);

}

#endif
