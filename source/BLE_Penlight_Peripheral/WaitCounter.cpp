#include "WaitCounter.h"

WaitCounter::WaitCounter(int32_t wait_ms)
{
    reset(wait_ms);
}

void WaitCounter::reset(int32_t wait_ms)
{
    wait_time  = wait_ms;
    reset_time = millis();
}

bool WaitCounter::isCounted(void)
{
    if (0 <= wait_time)
    {
        uint32_t current_time = millis();

        /* 長時間の動作は対象外という事にしてmillisのカウント値のオーバーフローは考慮していないが */
        /* unsignedの引き算の結果がオーバーフローして巨大な値になり、結果的にtrueに転ぶ気がする  */
        if ((uint32_t)wait_time <= (current_time - reset_time))
        {
            return true;
        }
    }

    return false;
}
