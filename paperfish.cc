#include "magic.h"
#include "table.h"
#include "uci.h"

int main() {
    engine::magic::initMagics();
    engine::initZobristTables();
    engine::UCI p;
    p.play();
    return 0;
}

