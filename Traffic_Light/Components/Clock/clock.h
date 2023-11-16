#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

typedef struct
{
    uint32_t year;
    uint32_t month;
    uint32_t day;
}clock_data_s_t;

typedef struct
{
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
}clock_time_s_t;

typedef struct
{
    clock_data_s_t data;
    clock_time_s_t time;
}clock_data_time_s_t;

void clock_init(void);
void clock_set_data_time(clock_data_time_s_t const * data_time);
void clock_get_data_time(clock_data_time_s_t * data_time);
void clock_set_alarm(clock_time_s_t * time);

#endif // CLOCK_H
