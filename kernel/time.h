#pragma once
#include <stdint.h>
#include <stddef.h>
#include "idt/interrupts.h"

struct time
{
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  weekDay;
    uint8_t  day;
    uint8_t  month;
    uint16_t year;
};

/**
 * @brief Gets the current time from the RTC
 * 
 * @return the current time in a time struct
 */
time GetTime();

/**
 * @brief sleeps for the given amount of seconds
 * 
 * @param seconds how long you want to sleep for
 */
void sleep(uint8_t seconds);