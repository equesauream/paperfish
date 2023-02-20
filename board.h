#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "move.h"
#include "position.h"
#include "time.h"

using U64 = unsigned long long;
using Key = unsigned long long;

namespace engine {

class Board {
  enum gameResult {
    whiteWin,
    blackWin,
    draw,
  };

  public:
    Position current;
    std::vector<Position> history;

    bool gameOver = false;
    gameResult gameRes = draw;

  private:
    std::unordered_map<Position, size_t, ZHash> seenPositions;
    size_t fiftyMove = 0;


  public:
    Board();
    Board(const std::string& FEN);
    ~Board();

    void move(const Move& m);
    void unmove();

    // search n ply or until quiescence
    void searchMoves(int n);

    void searchmove(const Move& m, int n);

    void searchWithTime(Time t);

    // quiescence search
    int quiesce(int alpha, int beta);

    int perft(int n);
    void perftDivide(int n);

  private:

    // for quiescence checks
    // currently just checking if the last move made was a capture,
    // or if the current position is a check
    // ideas to implement: check if a piece is hanging with future idea
    // to extend to pressure imbalance detector
    bool isQuiet();

    int alphabeta(int depth, int alpha, int beta, bool maximizingPlayer);

    // alpha-beta search
    int alphabetaMax(int depth, int alpha, int beta);
    int alphabetaMin(int depth, int alpha, int beta);

    
};

}

#endif
