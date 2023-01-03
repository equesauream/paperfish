#include "position.h"
#include "move.h"
#include "board.h"

#include <cmath>
#include <bitset>
#include <time.h>

using namespace engine;

int main () {
  //Board b = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  //Board b = Board();
  Board b("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
  // "8/5p2/8/8/8/8/5P2/4K1r1 b KQkq - 0 1"
  //Position p = Position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  //Position p = Position("r3k3/r3b3/3R1R2/8/8/8/8/4R3 b KQkq - 0 1");

  std::cout << b.current << '\n';
  //std::cout << b.current.castleRights.at('k');
  int k = 4;
  clock_t start_time = clock();
  std::cout << b.perft(k) << '\n';
  //b.perftDivide(k);
  start_time = clock() - start_time;
  std::cout << "done in " << ((double) start_time)/CLOCKS_PER_SEC << " seconds\n";
  /*for (auto& i : b.current.legalMoves()) {
    std::cout << i << '\n';
  }*/

  //std::cout << b.current.toFEN() << '\n';
  
  return 0;
}
