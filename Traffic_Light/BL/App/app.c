#include "app.h"
#include "light.h"
#include "timer.h"
#include "button.h"
#include "stm32f4xx_hal.h"
#include "flag_machine.h"
#include "led_display_auto.h"
#include "buzzer.h"
#include "clock.h"
#include "algorithm.h"
#include "debuger.h"

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

	algorithm_init();

	/* Need activate from RTC */
	fm_set_flag_with_delay(FLAG_TRAFIC_LIGHT_WARNING, 6000);
	
	debuger_init();
	/* For Example */
	led_display_auto_time(LED_DISPLAY_AUTO_RED_TIME, 25);
	led_display_auto_time(LED_DISPLAY_AUTO_GREEN_TIME, 15);
}	

void app_run(void)
{
	while (1)
	{
		if (fm_is_flag_set(FLAG_TRAFIC_LIGHT_WARNING))
		{
			fm_set_flag(FLAG_YELOW_BLINK);
			/* Need activate from RTC */
			fm_set_flag_with_delay(FLAG_TRAFIC_LIGHT_ENABLE, 6000);
			
			fm_clear_flag(FLAG_TRAFIC_LIGHT_WARNING);
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
		}
		if (fm_is_flag_set(FLAG_YELOW_BLINK))
		{
			algorithm_yelow_blink();
			fm_clear_flag(FLAG_YELOW_BLINK);
			fm_set_flag_with_delay(FLAG_YELOW_BLINK, TIME_BLINK);
		}
		if (fm_is_flag_set(FLAG_BUTTON_PRESSED))
		{
			if (!fm_is_flag_set(FLAG_BUTTON_LOCK))
			{
				fm_set_flag(FLAG_ALGORITHM_AUTO_NEXT_STEP);
				fm_clear_flag(FLAG_BUTTON_PRESSED);
				fm_set_flag(FLAG_BUTTON_LOCK);
			}
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
	}
}
