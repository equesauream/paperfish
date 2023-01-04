#include "position.h"
#include "move.h"
#include "board.h"

#include <cmath>
#include <bitset>
#include <time.h>

using namespace engine;

int main () {
  Board b;

  std::cout << b.current << '\n';
  //std::cout << b.current.castleRights.at('k');
  int k = 4;
  clock_t start_time = clock();
  const auto perft_result = b.perft(k);
  std::cout << perft_result << '\n';
  // b.perftDivide(k);
  start_time = clock() - start_time;
  const auto elapsed_seconds = ((double)start_time) / CLOCKS_PER_SEC;
  std::cout << "Done in " << elapsed_seconds << " seconds\n";

  const auto positions_per_second = perft_result / elapsed_seconds;
  std::cout << "Positions per second: " << positions_per_second << std::endl;
  
  return 0;
}
