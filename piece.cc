#include "piece.h"

namespace engine {

bool isWhite(Piece p) {
    return  p == 0 ||
            p == 1 ||
            p == 2 ||
            p == 3 ||
            p == 4 ||
            p == 5;
}

bool isBlack(Piece p) {
    return  p == 6 ||
            p == 7 ||
            p == 8 ||
            p == 9 ||
            p == 10 ||
            p == 11;
}

bool isPawn(Piece p) {
    return  p == 0 || p == 6;
}

bool isKnight(Piece p) {
    return  p == 1 || p == 7;
}

bool isBishop(Piece p) {
    return  p == 2 || p == 8;
}

bool isRook(Piece p) {
    return  p == 3 || p == 9;
}

bool isQueen(Piece p) {
    return  p == 4 || p == 10;
}

bool isKing(Piece p) {
    return  p == 5 || p == 11;
}

}