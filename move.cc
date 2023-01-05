#include "move.h"
#include "square.h"
#include <iostream>

namespace engine {

Move::Move(Square s, Square d, char p) : Move(s, d, p, '-') {}

Move::Move(Square s, Square d, char p, char prom) {
  piece = p;
  source = s;
  dest = d;
  promotionPiece = prom;

  castleRights = 0b1111;

  if (source == E1 || source == H1 || dest == E1 || dest == H1)
    castleRights &= ~(1 << 3);
  if (source == E1 || source == A1 || dest == E1 || dest == A1)
    castleRights &= ~(1 << 2);
  if (source == E8 || source == H8 || dest == E8 || dest == H8)
    castleRights &= ~(1 << 1);
  if (source == E8 || source == A8 || dest == E8 || dest == A8)
    castleRights &= ~(1 << 0);

  if (p == 'p' && rowNumber(source) == 7 && rowNumber(dest) == 5)
    enPassentSquare = source << 8;
  else if (p == 'P' && rowNumber(source) == 2 && rowNumber(dest) == 4)
    enPassentSquare = source >> 8;
  else
    enPassentSquare = NoSquare;
}

bool operator==(const Move &lhs, const Move &rhs) {
  return lhs.source == rhs.source && lhs.dest == rhs.dest;
}

std::ostream &operator<<(std::ostream &out, const Move &m) {
  out << bitToSquare(m.source) << bitToSquare(m.dest);
  if (m.promotionPiece != '-')
    out << m.promotionPiece;

  return out;
}
} // namespace engine
