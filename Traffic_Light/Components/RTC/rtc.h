
#include <stdint.h>

typedef struct
{
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t hours;
	uint32_t minutes;
	uint32_t seconds;
}rtc_data_time_s_t;

typedef struct
{
	uint32_t year;
	uint32_t month;
	uint32_t day;
}rtc_data_s_t;

typedef struct
{
	uint32_t hours;
	uint32_t minutes;
	uint32_t seconds;
}rtc_time_s_t;

void rtc_init(void);
void rtc_set_time(rtc_data_time_s_t const * data_time);
void rtc_get_time(rtc_data_time_s_t * data_time);
void rtc_set_alarm(rtc_data_time_s_t * data_time);