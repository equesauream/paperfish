#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "move.h"

using U64 = unsigned long long;
using Square = unsigned long long;
using Key = unsigned long long;
using Piece = int;
using MoveType = uint8_t;
/*

top left = first bit
top right = 8th bit
bottom left = 57th bit
bottom right = last bit

8   0 0 0 0 0 0 0 0
7   0 0 0 0 0 0 0 0
6   0 0 0 0 0 0 0 0
5   0 0 0 0 0 0 0 0
4   0 0 0 0 0 0 0 0
3   0 0 0 0 0 0 0 0
2   0 0 0 0 0 0 0 0
1   0 0 0 0 0 0 0 0

    A B C D E F G H
Order:
    White: P N B R Q K
    Black: p n b r q k
*/

namespace engine {

class Position {
  public:
    Key key;
    Key Okey;

  public:
    // the first 12 bitmaps (index 0-11) are the positions of the pieces in the standard order above
    std::vector<U64> thePosition;
    // whitePieces is the bitunion of all white piece bitboards, same with black
    U64 whitePieces;
    U64 blackPieces;
  private:
    // original position
    std::vector<U64> OthePosition;
    U64 OwhitePieces;
    U64 OblackPieces;
  public:
    // maps whitePawn to 0, whiteKnight to 1, etc.
    static const std::map<char, Piece> pieceMap;
    // reverse of above
    static const std::map<Piece, char> intMap;

    // 0 for white to move, 1 for black to move
    Colour turn;
    // bitmask of 4 bits e.g. 1101 corresponds to KQq
    uint8_t castleRights;
    // stores the square of en passent, if available
    Square enPassant;
    // number of half moves since last pawn move or capture
    int halfMove;
    // increments each time black moves
    int fullMove;

 private:
    bool Oturn;
    uint8_t OcastleRights;
    Square OenPassant;
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
    // get the FEN of the current position
    std::string toFEN() const;

    // for zobrist hashing
    bool operator==(const Position &other) const;

    
    // given a string, return a Move
    Move parseString(std::string s);

    MoveType getMoveType(Square source, Square dest, Piece p) const;


  public:

    // given a move m, check if it is a valid move wrt the current position
    bool isValid(const Move& m);
    // private helper method
  //private:
    bool isValidMove(const Move& m, Piece piece);
    // returns a list of possible moves for a piece p at square pos
    // i.e. the possible premoves you can make
    // private method
    std::vector<Move> possibleMoves(Piece p, Square pos) const;
    // returns a list of valid moves wrt the current position
  public:
    std::vector<Move> validMoves(Piece p, Square pos);
    // returns a list of all legal moves at the current position
  public:
    std::vector<Move> legalMoves();
    // updates the whitePieces and blackPieces bitboards wrt thePosition
    // private method
  private:
    void updateBoards();
    // sets the original position state to the current state
  public:
    void resetOriginal();

    // move the position
    void move(const Move& m);
    // reset the position
    void unmove();

    // moves the position and updates the original
    void advance(const Move& m);

    // returns if white/black is in check/checkmate
    bool whiteInCheck();
    bool blackInCheck();
    bool whiteInCheckmate();
    bool blackInCheckmate();

    
    // P/p = 100, N/n = 300, B/b = 300, R/r = 500, Q/q = 900
    // returns the material difference (positive = white, negative = black)
    int materialCount() const;
    int pawnStructure(Colour c) const;
    // get the heuristical value of a position
  public:
    // currently returns materialCount(), ideas to extend
    // by examining pawn structure (connectedness, doubledness)
    // also can tie to pressure imbalance detector
    // and synergy with bishop (good/bad bishop)
    int heurVal();


    friend std::ostream& operator<<(std::ostream& out, const Position& pos);
};

std::ostream& operator<<(std::ostream& out, const Position& pos);

}

#endif
