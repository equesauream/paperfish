#include "position.h"
#include "move.h"
#include "board.h"
#include "table.h"
#include "square.h"
#include "magic.h"

#include <time.h>
#include <bitset>

using namespace engine;

int main () {

  magic::initMagics();
  engine::initZobristTables();

  //return 0;



  Board b;
  //b.move(Move(E2, E4, whitePawn, Basic));
  //b.move(Move(E7, E5, blackPawn, Basic));
  //Board b("2R5/8/8/4k3/2Q5/7P/5PP1/6K1 b - - 0 1");
  //Board b("r7/5pp1/p7/1p6/2k1p3/P3b3/1PKBq1p1/3R4 w - - 0 1");
  //Board b("5k2/Q7/2p4p/pP3p2/PP1B1P2/P1NP4/2P4P/4R1K1 b - - 0 1");
  //Board b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  //Board b("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  //magic::printBitboard(magic::getChecks(whiteKing, b.current.thePosition.at(blackKing), b.current.whitePieces | b.current.blackPieces));
  //b.move(Move(C7, C5, blackPawn));
  //b.move(Move(B5, C6, whitePawn));

  //std::cout << b.current.pawnSupport(b.current.thePosition.at(0), White) << '\n';

  
  //b.searchMoves(6);

  int c = 0;
  for (int i = 0; i < 10; ++i) {
    ZHash z;
        std::cout << i << " \n" << std::bitset<64>(z(b.current)) << "\n" << std::bitset<64>(b.current.key) << std::endl; 
    int size = b.current.legalMoves().size();
    if (size > 0) {
      int r = rando(size - 1);
      b.move(b.current.legalMoves().at(r));
      if (z(b.current) != b.current.key) {
        ++c;
      }
    }
  }
  std::cout << c << std::endl;
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
