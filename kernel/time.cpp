#include "time.h"

#define REG_SEC                         0x00
#define REG_SEC_ALARM                   0x01
#define REG_MIN                         0x02
#define REG_MIN_ALARM                   0x03
#define REG_HOUR                        0x04
#define REG_HOUR_ALARM                  0x05
#define REG_WEEK_DAY                    0x06
#define REG_DAY                         0x07
#define REG_MONTH                       0x08
#define REG_YEAR                        0x09
#define REG_A                           0x0a
#define REG_B                           0x0b
#define REG_C                           0x0c
#define REG_D                           0x0d

#define IO_RTC_INDEX                    0x70
#define IO_RTC_TARGET                   0x71

#define REGB_DM                         (1 << 2)    // Data Mode (0 = BCD, 1 = Binary)

static uint8_t BcdToBin(uint8_t val)
{
    return (val & 0xf) + (val >> 4) * 10;
}

static uint8_t RtcRead(uint8_t addr)
{
    outb(IO_RTC_INDEX, addr);
    return inb(IO_RTC_TARGET);
}

void sleep(uint8_t seconds)
{
    time oldtime = GetTime();
    time CurrentTime = oldtime;

    while (CurrentTime.sec < oldtime.sec + seconds)
    {
        CurrentTime = GetTime();    
    }
}

time GetTime()
{
    uint8_t  sec = RtcRead(REG_SEC);
    uint8_t  min = RtcRead(REG_MIN);
    uint8_t  hour = RtcRead(REG_HOUR);
    uint8_t  weekDay = RtcRead(REG_WEEK_DAY);
    uint8_t  day = RtcRead(REG_DAY);
    uint8_t  month = RtcRead(REG_MONTH);
    uint16_t year = RtcRead(REG_YEAR);

    uint8_t regb = RtcRead(REG_B);

    if (~regb & REGB_DM)
    {
        sec = BcdToBin(sec);
        min = BcdToBin(min);
        hour = BcdToBin(hour);
        day = BcdToBin(day);
        month = BcdToBin(month);
        year = BcdToBin(year);
    }
    // Century support
    year += 2000;

    // Week day conversion: Sunday as the first day of the week (0-6)
    weekDay--;

    time CurrentTime;
    CurrentTime.weekDay = weekDay;
    CurrentTime.day = day;
    CurrentTime.month = month;
    CurrentTime.year = year;
    CurrentTime.hour = hour;
    CurrentTime.min = min;
    CurrentTime.sec = sec;

    return CurrentTime;
}