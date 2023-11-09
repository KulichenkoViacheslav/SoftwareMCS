#include "algorithm.h"
#include "light.h"
#include "buzzer.h"
#include "flag_machine.h"
#include <stdbool.h>

typedef enum {
    sound_norm  =  0,
    sound_quick =  1,
	sound_faster = 2,
    sound_max
}algorithm_sound_periodic_e_t;

static bool blinker_switch = false;
static algorithm_auto_step_e_t auto_trafic_light_mode = red;
static algorithm_pedestrian_step_e_t pedestrian_trafic_light_mode = pedestrian_red;
static uint8_t trafic_light_blink = 0;
static uint8_t pedestrian_blink_count = 0;
static uint32_t times_trafic_light[max_light_mode] = {TIME_RED, TIME_RED_YELOW, TIME_GREEN, TIME_GREEN_BLINK, TIME_YELOW};
static algorithm_sound_periodic_e_t sound_periodic = sound_norm;
static bool sound_active = false;
static uint32_t time_sound_frequency[] = {1800, 1300, 800};

void algorithm_init(void)
{
	light_set_color_state(auto_trafic_light, light_all, light_off);
	light_set_color_state(pedestrian_trafic_light, light_all, light_off);
}

void algorithm_yelow_blink(void)
{
	if (blinker_switch)
	{
		light_set_color_state(auto_trafic_light, light_yelow, light_on);
		blinker_switch = false;
	}
	else
	{
		light_set_color_state(auto_trafic_light, light_yelow, light_off);
		blinker_switch = true;
	}
}

void algorithm_auto(void)
{
	switch (auto_trafic_light_mode)
	{		
		case red:
		{
			light_set_color_state(auto_trafic_light, light_all, light_off);
			light_set_color_state(auto_trafic_light, light_red, light_on);
			fm_set_flag_with_delay(FLAG_ALGORITHM_AUTO_NEXT_STEP, times_trafic_light[auto_trafic_light_mode]);
			
			/* Config fo pedestrian trafic light*/
			pedestrian_trafic_light_mode = pedestrian_green;
			fm_set_flag_with_delay(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP, TIME_STOP_ALL_TRAFIC);
			
			/* Next step parameters */
			auto_trafic_light_mode = red_yelow;
			break;
		}
		case red_yelow:
		{
			light_set_color_state(auto_trafic_light, light_yelow, light_on);
			fm_set_flag_with_delay(FLAG_ALGORITHM_AUTO_NEXT_STEP, times_trafic_light[auto_trafic_light_mode]);

			/* Next step parameters */
			auto_trafic_light_mode = green;
			break;
		}
		/* Start cycle */
		case green:
		{			
			light_set_color_state(auto_trafic_light, light_all, light_off);
			light_set_color_state(auto_trafic_light, light_green, light_on);
			fm_set_flag(FLAG_BUTTON_CHECK);
			fm_set_flag_with_delay(FLAG_BUTTON_UNLOCK, 40000);
			/* Next step parameters */
			auto_trafic_light_mode = green_blink;
			trafic_light_blink = 8;
			break;
		}
		case green_blink:
		{
			if ((trafic_light_blink % 2) == 0)
			{
				light_set_color_state(auto_trafic_light, light_green, light_off);
			}
			else
			{
				light_set_color_state(auto_trafic_light, light_green, light_on);
			}
			trafic_light_blink --;
			if (trafic_light_blink == 0)
			{
				/* Next step parameters */
				auto_trafic_light_mode = yelow;
				fm_set_flag(FLAG_ALGORITHM_AUTO_NEXT_STEP);
			}
			else
			{
				fm_set_flag_with_delay(FLAG_ALGORITHM_AUTO_NEXT_STEP, 500);
			}
			break;
		}
		case yelow:
		{
			light_set_color_state(auto_trafic_light, light_green, light_off);
			light_set_color_state(auto_trafic_light, light_yelow, light_on);
			fm_set_flag_with_delay(FLAG_ALGORITHM_AUTO_NEXT_STEP, times_trafic_light[auto_trafic_light_mode]);

			/* Next step parameters */
			auto_trafic_light_mode = red;
			break;
		}
		default:
			break;
	}
}

void algorithm_auto_set_step(algorithm_auto_step_e_t step)
{
	auto_trafic_light_mode = step;
}

void algorithm_pedestrian(void)
{
	switch (pedestrian_trafic_light_mode)
    {
		case pedestrian_red:
        {            
            light_set_color_state(pedestrian_trafic_light, light_red, light_on);                       
			pedestrian_trafic_light_mode = pedestrian_green;
            break;
        }
		case pedestrian_green:
		{
            light_set_color_state(pedestrian_trafic_light, light_green, light_on);
			
			uint32_t time = times_trafic_light[red] - 2 * TIME_STOP_ALL_TRAFIC - TIME_GREEN_BLINK;
			fm_set_flag_with_delay(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP, time);
			
			fm_set_flag(FLAG_AUDIBLE_SIGNAL_ON);
			sound_periodic = sound_norm;
			fm_set_flag_with_delay(FLAG_AUDIBLE_SIGNAL_PERIODICAL_CHANGE, time - TIME_SOUND_PERIODIC_CHANGE);
			
            pedestrian_blink_count = 0;
            pedestrian_trafic_light_mode = pedestrian_blink;
            break;
		}
		case pedestrian_blink:
        {
			if (pedestrian_blink_count % 2 == 0)
            {                
                light_set_color_state(pedestrian_trafic_light, light_green, light_off);
            }
			else
            {               
                light_set_color_state(pedestrian_trafic_light, light_green, light_on);
            }           
            pedestrian_blink_count++;
            
			if (pedestrian_blink_count > 8)  
            {             
                light_set_color_state(pedestrian_trafic_light, light_green, light_off);               
                pedestrian_trafic_light_mode = pedestrian_red;
				fm_set_flag(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP);
            }
			else
            {
                fm_set_flag_with_delay(FLAG_ALGORITHM_PEDESTRIAN_NEXT_STEP, 500);
            }
			break;
		}
	}	
}

void algorithm_pedestrian_set_step(algorithm_pedestrian_step_e_t step)
{
	pedestrian_trafic_light_mode = step;
}

void algorithm_sound(void)
{
	if (sound_active == true)
	{
		buzzer_set(sound_on);
		fm_set_flag_with_delay(FLAG_AUDIBLE_SIGNAL_ON, TIME_SOUND_ACTIVE);
		sound_active = false;
	}
	else
	{
		buzzer_set(sound_off);
		fm_set_flag_with_delay(FLAG_AUDIBLE_SIGNAL_ON, time_sound_frequency[sound_periodic]);
		sound_active = true;
	}
}

void algorithm_sound_set_next_step(void)
{
	sound_periodic ++;
	if (sound_periodic < sound_max)
	{
		fm_set_flag_with_delay(FLAG_AUDIBLE_SIGNAL_PERIODICAL_CHANGE, TIME_SOUND_PERIODIC_CHANGE);
	}
	else
	{
		fm_clear_flag(FLAG_AUDIBLE_SIGNAL_ON);
		fm_clear_flag_with_delay(FLAG_AUDIBLE_SIGNAL_ON);
	}
}