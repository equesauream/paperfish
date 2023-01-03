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

    // maps 'K','Q', 'k', 'q' to T/F
    std::map<char, bool> castleRights; // true for enabled
    // enabled if a pawn takes a double step, `-` if not
    std::string enPassentSquare;
    char promotionPiece = '-';
    
  public:
    Move(std::string move, char piece);
};

}

#endif
