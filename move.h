#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <vector>
#include <map>

typedef unsigned long long U64;

namespace engine {

class Move {
  public:
    std::string source;
    std::string dest;    
    char piece;

    // bitmask of 4 bits e.g. 1101 for KQq
    int castleRights; // true for enabled
    // enabled if a pawn takes a double step, `-` if not
    std::string enPassentSquare;
    char promotionPiece = '-';
    
  public:
    Move(std::string move, char piece);
};

}

#endif
