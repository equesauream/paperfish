#include "move.h"
#include "square.h"
#include "position.h"
#include <iostream>

namespace engine {

Move::Move() {
    source = 0;
    dest = 0;
    piece = '-';
    castleRights = 0b0000;
    enPassantSquare = 0;
}

Move::Move(Square s, Square d, char p, char prom) {
    piece  = p;
    source = getSquareIndex(s);
    dest   = getSquareIndex(d);
    promotionPiece = prom;

    castleRights = 0b1111;

    if ((s | d) & (E1 | H1)) 
        castleRights &= ~(1 << 3);
    if ((s | d) & (E1 | A1)) 
        castleRights &= ~(1 << 2);
    if ((s | d) & (E8 | H8)) 
        castleRights &= ~(1 << 1);
    if ((s | d) & (E8 | A8))
        castleRights &= ~(1 << 0);

    if (p == 'p' && rowNumber(s) == 7 && rowNumber(d) == 5)
        enPassantSquare = s << 8;
    else if (p == 'P' && rowNumber(s) == 2 && rowNumber(d) == 4)
        enPassantSquare = s >> 8;
    else 
        enPassantSquare = 0;
}

bool operator==(const Move& lhs, const Move& rhs) {
    return lhs.source == rhs.source && lhs.dest == rhs.dest;
}

std::ostream& operator<<(std::ostream& out, const Move& m) {
    out << bitToSquare(getSquare(m.source)) << bitToSquare(getSquare(m.dest));
    if (m.promotionPiece != '-')
        out << (char) tolower(Position::intMap.at(m.promotionPiece));
    
    return out;
}

}

