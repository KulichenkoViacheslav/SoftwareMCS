#ifndef ALGORITHM_H
#define ALGORITHM_H

#define TIME_RED 15000
#define TIME_RED_YELOW 3000
#define TIME_GREEN 30000
#define TIME_GREEN_BLINK 4000
#define TIME_YELOW 3000
#define TIME_STOP_ALL_TRAFIC 2000
#define TIME_SOUND_ACTIVE 200
#define TIME_SOUND_PERIODIC_CHANGE 4000
#define TIME_BLINK 500

typedef enum
{
    red = 0,
    red_yelow,
    green,
    green_blink,
    yelow,
    max_light_mode
}algorithm_auto_step_e_t;

typedef enum {
    pedestrian_red = 0,
    pedestrian_green,
    pedestrian_blink,
}algorithm_pedestrian_step_e_t;

void algorithm_init(void);
void algorithm_yelow_blink(void);
void algorithm_auto(void);
void algorithm_auto_set_step(algorithm_auto_step_e_t step);
void algorithm_pedestrian(void);
void algorithm_pedestrian_set_step(algorithm_pedestrian_step_e_t step);
void algorithm_sound(void);
void algorithm_sound_set_next_step(void);
void algorithm_display_change(void);
#endif