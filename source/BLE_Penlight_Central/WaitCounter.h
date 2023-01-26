// for wait time
#ifndef WAIT_COUNTER_H
#define WAIT_COUNTER_H

#include <Arduino.h>

// counter for waiting exactly
class WaitCounter
{
    int32_t  wait_time;
    uint32_t reset_time;

public:
    WaitCounter(int32_t wait_ms=0);
    void reset(int32_t wait_ms);
    bool isCounted(void);
};

#endif // WAIT_COUNTER_H
