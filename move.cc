#include "move.h"
#include <iostream>

namespace engine {

Move::Move(std::string move, char p) {
    piece = p;
    source = move.substr(0, 2);
    dest = move.substr(2, 2);
    if (move.length() > 4) promotionPiece = move.at(5);

    castleRights = 0b1111;

    if (source == "e1" || source == "h1" || dest == "e1" || dest == "h1") castleRights &= ~(1 << 3);
    if (source == "e1" || source == "a1" || dest == "e1" || dest == "a1") castleRights &= ~(1 << 2);
    if (source == "e8" || source == "h8" || dest == "e8" || dest == "h8") castleRights &= ~(1 << 1);
    if (source == "e8" || source == "a8" || dest == "e8" || dest == "a8") castleRights &= ~(1 << 0);

    if (p == 'p' && source.at(1) == '7' && dest.at(1) == '5') enPassentSquare = std::string(1, dest.at(0)) + "6";
    else if (p == 'P' && source.at(1) == '2' && dest.at(1) == '4') enPassentSquare = std::string(1, dest.at(0)) + "3";
    else enPassentSquare = "-";
}

}

