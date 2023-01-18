#include "position.h"
#include "move.h"
#include "board.h"
#include "table.h"
#include "square.h"
#include "magic.h"

#include <time.h>

using namespace engine;

int main () {

  magic::initMagics();

  // printBitboard(getAttacks(blackRook, A7, A8 | A7 | C7));

  //return 0;


  //Board b;
  Board b("5k2/Q7/2p4p/p4p2/3B1P2/P1NP4/1PP3PP/4R1K1 b - - 0 1");
  //Board b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  //Board b("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  //b.move(Move(E2, E4, whitePawn));
  //b.move(Move(C7, C5, blackPawn));
  //b.move(Move(B5, C6, whitePawn));

  b.searchMoves(5);
  std::cout << b.current.heurVal() << '\n';

  std::cout << transTable[b.current].bestMove << ": " << transTable[b.current].eval <<'\n';

  return 0;
  
  int k = 6;
  //b.perftDivide(k);
  clock_t start_time = clock();
  const auto perft_result = b.perft(k);
  std::cout << perft_result << '\n';
  start_time = clock() - start_time;
  const auto elapsed_seconds = ((double) start_time) / CLOCKS_PER_SEC;
  std::cout << "Done in " << elapsed_seconds << " seconds\n";

  const auto positions_per_second = perft_result / elapsed_seconds;
  std::cout << "Positions per second: " << positions_per_second << std::endl;
  
  return 0;
}
