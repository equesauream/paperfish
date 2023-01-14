#ifndef MAGIC_H
#define MAGIC_

#include <vector>

namespace engine {

//                     BE CAREFUL                       //
//         YOU ARE ENTERING THE ZONE OF MAGIC           //
//               ENTER AT YOUR OWN RISK                 //


namespace magic {

using U64 = unsigned long long;
using Piece = int;
using Square = unsigned long long;

// only need to call this function
void initMagics();

void initRookTable();
void initBishopTable();

void initPawnAttacks();
void initKnightAttacks();
// a white king on E1 can attack C1 and G1
void initKingAttacks();
void initRookAttacks();
void initBishopAttacks();

void printBitboard(U64 b);

std::vector<U64> generateBlockers(U64 bits);
// blockers will be the union of whitePieces' and blackPieces' positions
U64 getAttacks(Piece p, Square s, U64 blockers);

}

}

#endif
