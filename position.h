#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>


#include "move.h"

using U64 = unsigned long long;
using Square = long long unsigned;
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
  // the first 12 bitmaps (index 0-11) are the positions of the pieces in the
  // standard order above
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
  // maps 'P' to 0, 'N' to 1, etc.
  static const std::map<char, int> pieceMap;
  // reverse of above
  static const std::map<int, char> intMap;

  // 0 for white to move, 1 for black to move
  bool turn;
  // bitmask of 4 bits e.g. 1101 corresponds to KQq
  int castleRights;
  // stores the square of en passent, if available
  Square enPassent;
  // number of half moves since last pawn move or capture
  int halfMove;
  // increments each time black moves
  int fullMove;

private:
  bool Oturn;
  int OcastleRights;
  Square OenPassent;
  int OhalfMove;
  int OfullMove;

public:
  // starting position
  Position();
  Position(const std::string &FEN);
  Position(const Position &other);
  Position(const Position &&other);
  Position &operator=(Position other);
  // converts FEN to Position
  ~Position();
  // get the FEN of the current position
  std::string toFEN() const;

public:
  // returns a vector of all set squares of a bitboard
  static std::vector<Square> bbToSquares(U64 board);

  //  given a move m, check if it is a valid move wrt the current position
  bool isValid(const Move &m);
  // private helper method method
  bool isValidMove(const Move &m, char piece, char orig);
  // piece p e.g. 'n', position pos e.g. e7
  // private method to check if a move is blocked
  bool isMoveBlocked(const Move &m) const;
  // returns a list of possible moves for a piece p at square pos
  // i.e. the possible premoves you can make
  // private method
  std::vector<Move> possibleMoves(char p, Square pos) const;
  // returns a list of valid moves wrt the current position
  std::vector<Move> validMoves(char p, Square pos);
  // returns a list of squares that are attacked by a piece p at square pos
  // private method
  std::vector<Square> checkingSquares(char p, Square pos) const;
  // returns a list of all legal moves at the current position
  std::vector<Move> legalMoves();
  // updates the whitePieces and blackPieces bitboards wrt thePosition
  // private method
  void updateBoards();
  // sets the current position state to the original state
  // private method
  void resetOriginal();

  // move the position
  void move(Move m);
  // reset the position
  void unmove();

  // returns if white/black is in check/checkmate
  bool whiteInCheck();
  bool blackInCheck();
  bool whiteInCheckmate();
  bool blackInCheckmate();

  // P/p = 1, N/n = 3, B/b = 3, R/r = 5, Q/q = 9
  // returns the material difference (positive = white, negative = black)
  int materialCount() const;

  // returns the number of set bits
  static int countBits(U64 num);

  friend std::ostream &operator<<(std::ostream &out, const Position &pos);
};

std::ostream &operator<<(std::ostream &out, const Position &pos);

} // namespace engine

#endif
