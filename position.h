#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "move.h"

typedef unsigned long long U64;
/*

top left = first bit
top right = 8th bit
bottom left = 57th bit
bottom right = last bit

0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

Order:
    White: P N B R Q K
    Black: p n b r q k
*/

namespace engine {

class Position {
  public:
    // the first 12 bitmaps (index 0-11) are the positions of the pieces in the standard order above
    std::vector<U64> thePosition;
    U64 whitePieces;
    U64 blackPieces;
  private:
    // original position
    std::vector<U64> OthePosition;
    U64 OwhitePieces;
    U64 OblackPieces;
  public:
    // maps 'P' to 0, 'N' to 1, etc.
    static const std::map<char, int> pieceMap;
    // reverse of above
    static const std::map<int, char> intMap;

    // 0 for white to move, 1 for black to move
    bool turn;
    // bitmask of 4 bits e.g. 1101 corresponds to KQq
    int castleRights;
    // stores the square of en passent, if available
    std::string enPassent;
    // number of half moves since last pawn move or capture
    int halfMove;
    // increments each time black moves
    int fullMove;

 private:
    bool Oturn;
    int OcastleRights;
    std::string OenPassent;
    int OhalfMove;
    int OfullMove;
    

  public:
    // starting position
    Position();
    Position(const std::string& FEN);
    Position(const Position& other);
    Position(const Position&& other);
    Position& operator=(Position other);
    // converts FEN to Position
    ~Position();
    std::string toFEN() const;

  public:
    static std::string bitToSquare(U64 bit);
    static U64 squareToBit(const std::string& s);
    static std::vector<std::string> bbToSquares(U64 board);

    bool isValidMove(const std::string& s, char piece, char orig);
    // piece p e.g. 'n', position pos e.g. e7
    bool isMoveBlocked(const std::string& source, const std::string& dest) const;
    std::vector<std::string> possibleMoves(char p, const std::string& pos) const;
    std::vector<std::string> validMoves(char p, const std::string& pos);
    std::vector<std::string> checkingSquares(char p, const std::string& pos) const;  
    std::vector<std::string> legalMoves();
    void updateBoards();
    void resetOriginal();

    void move(Move m);
    void unmove();

    bool whiteInCheck();
    bool blackInCheck();
    bool whiteInCheckmate();
    bool blackInCheckmate();


    // P/p = 1, N/n = 3, B/b = 3, R/r = 5, Q/q = 9
    // returns the material difference (positive = white, negative = black)
    int materialCount() const;

    int countBits(U64 num) const;


    friend std::ostream& operator<<(std::ostream& out, const Position& pos);
};

std::ostream& operator<<(std::ostream& out, const Position& pos);

}

#endif
