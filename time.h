#ifndef TIME_H
#define TIME_H

#include <time.h>

namespace engine {

struct Time {
    clock_t remainingTime;
    Time(int seconds) : remainingTime{seconds * CLOCKS_PER_SEC} {}
};

}

#endif
