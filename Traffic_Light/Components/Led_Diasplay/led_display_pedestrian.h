#ifndef LED_DISPLAY_PEDESTRIAN_H
#define LED_DISPLAY_PEDESTRIAN_H

#include <stdint.h>

typedef enum
{
    LED_DISPLAY_PEDESTRIAN_RED_TIME,
    LED_DISPLAY_PEDESTRIAN_GREEN_TIME,
}led_display_pedestrian_color_time_e_t;

void led_display_pedestrian_init(void);
void led_display_pedestrian_clear(void);
void led_display_pedestrian_infinity(led_display_pedestrian_color_time_e_t color);
void led_display_pedestrian_time(led_display_pedestrian_color_time_e_t color, uint8_t time);

#endif