#include "app.h"
#include "light.h"
#include "timer.h"
#include "button.h"
#include "stm32f4xx_hal.h"
#include "flag_machine.h"
#include "led_display_auto.h"
#include "led_display_pedestrian.h"
#include "buzzer.h"
#include "clock.h"
#include "algorithm.h"
#include "debuger.h"
#include <stdio.h>

// Variants
/*
1. Red
2. Red + Yelow
3. Green
4. Green blink
5. Yelow
*/

void app_init(void)
{
    clock_init();
    timer_init();
    fm_init();
    light_init();
    led_display_auto_init();
    led_display_pedestrian_init();

    algorithm_init();
    
    debuger_init();
    
    printf("> Init compleated\r\n");
    printf("> Trafic Light is Diasble State\r\n");
}    

void app_run(void)
{
    while (1)
    {
        if (fm_is_flag_set(FLAG_TRAFIC_LIGHT_DISABLE))
        {
            fm_clear_flag(FLAG_TRAFIC_LIGHT_DISABLE);
            algorithm_init();
            printf("> Trafic Light is Disable State\r\n");
        }
        if (fm_is_flag_set(FLAG_TRAFIC_LIGHT_WARNING))
        {
            fm_set_flag(FLAG_YELOW_BLINK);
            fm_clear_flag(FLAG_TRAFIC_LIGHT_WARNING);
            algorithm_init();
            printf("> Trafic Light is Warning State\r\n");
        }
        if (fm_is_flag_set(FLAG_TRAFIC_LIGHT_ENABLE))
        {
            fm_clear_flag_with_delay(FLAG_YELOW_BLINK);
            /* Auto */
            algorithm_auto_set_step(green);
            fm_set_flag(FLAG_ALGORITHM_AUTO_NEXT_STEP);
            
            /* Pedestrian */
            algorithm_pedestrian_set_step(pedestrian_red);
            fm_set_flag(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP);
            
            fm_clear_flag(FLAG_TRAFIC_LIGHT_ENABLE);
            printf("> Trafic Light is Enable State\r\n");
        }
        if (fm_is_flag_set(FLAG_YELOW_BLINK))
        {
            algorithm_yelow_blink();
            fm_clear_flag(FLAG_YELOW_BLINK);
            fm_set_flag_with_delay(FLAG_YELOW_BLINK, TIME_BLINK);
        }
        if (fm_is_flag_set(FLAG_BUTTON_PRESSED))
        {
            fm_clear_flag(FLAG_BUTTON_PRESSED);
            
            /* Stop read pedestrian button */
            fm_clear_flag_with_delay(FLAG_BUTTON_CHECK);
            fm_clear_flag(FLAG_BUTTON_CHECK);
            
            algorithm_button_pressed();
        }
        if (fm_is_flag_set(FLAG_BUTTON_CHECK))
        {
            button_check();
            fm_clear_flag(FLAG_BUTTON_CHECK);
            fm_set_flag_with_delay(FLAG_BUTTON_CHECK, BUTTON_TIMEOUT_CHECK);
        }
        if (fm_is_flag_set(FLAG_BUTTON_UNLOCK))
        {
            fm_clear_flag(FLAG_BUTTON_LOCK);
            fm_clear_flag(FLAG_BUTTON_UNLOCK);
        }
        if (fm_is_flag_set(FLAG_ALGORITHM_AUTO_NEXT_STEP))
        {
            fm_clear_flag(FLAG_ALGORITHM_AUTO_NEXT_STEP);
            algorithm_auto();
        }
        if (fm_is_flag_set(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP))
        {
            fm_clear_flag(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP);
            algorithm_pedestrian();
        }
        if (fm_is_flag_set(FLAG_AUDIBLE_SIGNAL_ON))
        {
            fm_clear_flag(FLAG_AUDIBLE_SIGNAL_ON);
            algorithm_sound();
        }
        if (fm_is_flag_set(FLAG_AUDIBLE_SIGNAL_PERIODICAL_CHANGE))
        {
            fm_clear_flag(FLAG_AUDIBLE_SIGNAL_PERIODICAL_CHANGE);
            algorithm_sound_set_next_step();
        }
        if (fm_is_flag_set(FLAG_DEBUGER_RECEIVED_COMMAND))
        {
            fm_clear_flag(FLAG_DEBUGER_RECEIVED_COMMAND);
            debuger_execute_command();
        }
        if (fm_is_flag_set(FLAG_DISPLAY_AUTO_CHANGE))
        {
            fm_clear_flag(FLAG_DISPLAY_AUTO_CHANGE);
            algorithm_display_auto_change();
        }
        if (fm_is_flag_set(FLAG_DISPLAY_PEDESTRIAN_CHANGE))
        {
            fm_clear_flag(FLAG_DISPLAY_PEDESTRIAN_CHANGE);
            algorithm_display_pedestrian_change();
        }
    }
}
