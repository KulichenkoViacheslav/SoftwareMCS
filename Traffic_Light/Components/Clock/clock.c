#include "clock.h"
#include "rtc.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "flag_machine.h"

static clock_data_time_s_t data_time;

void clock_init(void)
{
    data_time.data.day = 2;
    data_time.data.month = 11;
    data_time.data.year = 2023;
    
    data_time.time.hours = 0;
    data_time.time.minutes = 0;
    data_time.time.seconds = 0;

    clock_set_data_time(&data_time);
}

void clock_set_data_time(clock_data_time_s_t const * data_time)
{
    RTC_DateTypeDef new_data = {0};
    new_data.Date = data_time->data.day;
    new_data.Month = data_time->data.month;
    new_data.Year = data_time->data.year - 2000;
    
    HAL_RTC_SetDate(&hrtc, &new_data, RTC_FORMAT_BCD);

    RTC_TimeTypeDef new_time = {0};
    new_time.Hours = data_time->time.hours;
    new_time.Minutes = data_time->time.minutes;
    new_time.Seconds = data_time->time.seconds;
    HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BCD);
}

void clock_get_data_time(clock_data_time_s_t * data_time)
{
    RTC_DateTypeDef cur_data = {0};
    HAL_RTC_GetDate(&hrtc, &cur_data, RTC_FORMAT_BCD);
    
    data_time->data.day = cur_data.Date;
    data_time->data.month = cur_data.Month;
    data_time->data.year = cur_data.Year + 2000;

    RTC_TimeTypeDef cur_time = {0};    
    HAL_RTC_GetDate(&hrtc, &cur_data, RTC_FORMAT_BCD);

    data_time->time.hours = cur_time.Hours;
    data_time->time.minutes = cur_time.Minutes;
    data_time->time.seconds = cur_time.Seconds;

}

void clock_set_alarm(clock_time_s_t * time)
{
    RTC_AlarmTypeDef alarm = {0};
    
    alarm.AlarmTime.Hours = time->hours;
    alarm.AlarmTime.Minutes = time->minutes;
    alarm.AlarmTime.Seconds = time->seconds;
    
    HAL_RTC_SetAlarm_IT(&hrtc, &alarm, RTC_FORMAT_BCD);
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  
}