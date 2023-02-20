#include <iostream>
#include <sstream>
#include <string>

#include "board.h"
#include "position.h"
#include "table.h"
#include "move.h"
#include "magic.h"
#include "uci.h"

using namespace engine;

int main() {
    magic::initMagics();
    initZobristTables();
    UCI p;
    p.play();
    return 0;
}

