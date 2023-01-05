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
    Board(const std::string& FEN);
    ~Board();

    void move(const Move& m);
    void unmove();

    int perft(int n);
    void perftDivide(int n);  
    

    // P/p = 1, N/n = 3, B/b = 3, R/r = 5, Q/q = 9
    // returns the material difference (positive = white, negative = black)
    int materialCount() const;
};

}

#endif
