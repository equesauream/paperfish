#ifndef MOVE_H
#define MOVE_H

#include <map>
#include <string>
#include <vector>


#include "square.h"
class position;

typedef unsigned long long U64;
using Square = unsigned long long;

namespace engine {

class Move {
public:
  Square source;
  Square dest;
  char piece;

  // bitmask of 4 bits e.g. 1101 for KQq
  int castleRights; // true for enabled
  // enabled if a pawn takes a double step, `-` if not
  Square enPassentSquare;
  // defaults if no promotion piece is '-'
  char promotionPiece = '-';

public:
  Move(Square s, Square d, char piece);
  Move(Square s, Square d, char piece, char prom);

  friend class Position;
  friend bool operator==(const Move &lhs, const Move &rhs);
  friend std::ostream &operator<<(std::ostream &out, const Move &m);
};

bool operator==(const Move &lhs, const Move &rhs);
std::ostream &operator<<(std::ostream &out, const Move &m);

} // namespace engine

#endif
