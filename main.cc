#include "position.h"
#include "move.h"
#include "board.h"

#include <cmath>
#include <bitset>
#include <time.h>

using namespace engine;

int main () {
  //Board b;
  //Board b("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  Board b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  std::cout << b.current << '\n';
  //std::cout << b.current.castleRights.at('k');
  int k = 5;
  clock_t start_time = clock();
  //b.perftDivide(k);
  const auto perft_result = b.perft(k);
  std::cout << perft_result << '\n';
  start_time = clock() - start_time;
  const auto elapsed_seconds = ((double) start_time) / CLOCKS_PER_SEC;
  std::cout << "Done in " << elapsed_seconds << " seconds\n";

  const auto positions_per_second = perft_result / elapsed_seconds;
  std::cout << "Positions per second: " << positions_per_second << std::endl;
  
  return 0;
}
