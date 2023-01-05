#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "move.h"
#include "position.h"

typedef unsigned long long U64;

namespace engine {

class Board {
public:
  Position current;
  std::vector<Position> history;

public:
  Board();
  Board(const std::string &FEN);
  ~Board();

  void move(const Move &m);
  void unmove();

  int perft(int n);
  void perftDivide(int n);
};

} // namespace engine

#endif
