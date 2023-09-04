#include "STC8G_H_Delay.h"

/// @brief Millisecond delay function. Can only be roughly delayed.
/// @param ms Delay time.(1 ms)
void delay_ms(u16 ms)
{
    u16 i;
    do {
        i = MAIN_Fosc / 10000;
        while (--i)
            ;
    } while (--ms);
}
