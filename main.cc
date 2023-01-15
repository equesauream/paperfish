#include "position.h"
#include "move.h"
#include "board.h"
#include "table.h"
#include "square.h"
#include "magic.h"

#include <cmath>
#include <bitset>
#include <time.h>

using namespace engine;
using namespace magic;

int main () {

  initMagics();

  // printBitboard(getAttacks(blackRook, A7, A8 | A7 | C7));

  //return 0;


  //Board b;
  //Board b("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  //Board b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  Board b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  // /b.move(Move(E2, E4, whitePawn));
  // /b.move(Move(C7, C5, blackPawn));
  // /b.move(Move(B5, C6, whitePawn));

  /*for (const auto& i : b.current.legalMoves()) {
    std::cout << i << '\n';
  }*/

  std::cout << b.current << '\n';
  
  int k = 4;
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
