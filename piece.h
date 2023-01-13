#ifndef PIECE_H
#define PIECE_H

using Piece = int;

namespace engine {

enum {
    whitePawn   = 0,
    whiteKnight = 1,
    whiteBishop = 2,
    whiteRook   = 3,
    whiteQueen  = 4,
    whiteKing   = 5,
    
    blackPawn   = 6,
    blackKnight = 7,
    blackBishop = 8,
    blackRook   = 9,
    blackQueen  = 10,
    blackKing   = 11,

    NoPiece
};

bool isWhite(Piece p);

bool isBlack(Piece p);

bool isPawn(Piece p);

bool isKnight(Piece p);

bool isBishop(Piece p);

bool isRook(Piece p);

bool isQueen(Piece p);

bool isKing(Piece p);

}

#endif
