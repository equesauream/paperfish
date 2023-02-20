#ifndef UCI_H
#define UCI_H

#include <vector>
#include <string>

#include "board.h"

namespace engine {

class UCI {
    struct handleGo {

    };
    Board game = Board();

  public:
    void play();
};

}

#endif
