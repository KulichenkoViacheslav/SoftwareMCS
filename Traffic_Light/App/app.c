#include "app.h"
#include "light.h"
#include "timer.h"
#include "button.h"
#include "stm32f4xx_hal.h"
#include "flag_machine.h"
#include "led_display_auto.h"
#include "buzzer.h"

// Variants
/*
1. Red
2. Red + Yelow
3. Green
4. Green blink
5. Yelow
*/

typedef enum {
    pedestrian_red = 0,
    pedestrian_green,
    pedestrian_blink,
}pedestrian_trafic_light_mode_e_t;

typedef enum
{
	disable = 0,
	enable,
	waiting,
}trafic_light_state_e_t;

typedef enum
{
	red = 0,
	red_yelow,
	green,
	green_blink,
	yelow,
	max_light_mode
}trafic_light_mode_e_t;

#define TIME_RED 15000
#define TIME_RED_YELOW 3000
#define TIME_GREEN 30000
#define TIME_GREEN_BLINK 4000
#define TIME_YELOW 3000
#define TIME_STOP_ALL_TRAFIC 2000
#define TIME_SOUND_ACTIVE 200

uint32_t times_trafic_light[max_light_mode] = {TIME_RED, TIME_RED_YELOW, TIME_GREEN, TIME_GREEN_BLINK, TIME_YELOW};
uint8_t  time_sound_index = 0;
uint32_t time_sound_frequency[] = {1800, 1300, 800};

static trafic_light_mode_e_t trafic_light_mode = red;
static trafic_light_state_e_t trafic_light_state = disable;
static pedestrian_trafic_light_mode_e_t pedestrian_trafic_light_mode = pedestrian_red;
static uint8_t trafic_light_blink = 0;
static uint8_t pedestrian_blink_count = 0;
static bool blinker_switch = false;

static void app_change_mode(void);
static void app_yelow_blink(void);
static void app_pedestrian_change_mode(void);

void app_init(void)
{
	timer_init();
	fm_init();
	light_init();
	led_display_auto_init();

	light_set_color_state(auto_trafic_light, light_all, light_off);
	light_set_color_state(pedestrian_trafic_light, light_all, light_off);
	
	trafic_light_state = disable;
	/* Need activate from RTC */
	fm_set_flag_with_delay(FLAG_TRAFIC_LIGHT_WAITING, 6000);	
}	

void app_run(void)
{
	while (1)
	{
		if (fm_is_flag_set(FLAG_BUTTON_PRESSED))
		{
			if (!fm_is_flag_set(FLAG_BUTTON_LOCK))
			{
				fm_set_flag(FLAG_CHANGE_MODE);
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
		if (fm_is_flag_set(FLAG_TRAFIC_LIGHT_WAITING))
		{
			trafic_light_state = waiting;
			fm_set_flag(FLAG_YELOW_BLINK);
			/* Need activate from RTC */
			fm_set_flag_with_delay(FLAG_TRAFIC_LIGHT_ENABLE, 6000);
			
			fm_clear_flag(FLAG_TRAFIC_LIGHT_WAITING);
		}
		if (fm_is_flag_set(FLAG_TRAFIC_LIGHT_ENABLE))
		{
			trafic_light_state = enable;
			fm_clear_flag_with_delay(FLAG_YELOW_BLINK);
			/* Auto */
			trafic_light_mode = green;
			fm_set_flag(FLAG_CHANGE_MODE);
			
			/* Pedestrian */
			pedestrian_trafic_light_mode = pedestrian_red;
			fm_set_flag(FLAG_PEDESTRIAN_CHANGE_MODE);
			
			fm_clear_flag(FLAG_TRAFIC_LIGHT_ENABLE);
		}
		if (fm_is_flag_set(FLAG_YELOW_BLINK))
		{
			app_yelow_blink();
			fm_clear_flag(FLAG_YELOW_BLINK);
			fm_set_flag_with_delay(FLAG_YELOW_BLINK, 500);
		}
		if (fm_is_flag_set(FLAG_CHANGE_MODE))
		{
			fm_clear_flag(FLAG_CHANGE_MODE);
			app_change_mode();
		}
		if (fm_is_flag_set(FLAG_PEDESTRIAN_CHANGE_MODE))
		{
			fm_clear_flag(FLAG_PEDESTRIAN_CHANGE_MODE);
			app_pedestrian_change_mode();
		}
		if (fm_is_flag_set(FLAG_BUZZER_ON))
		{
			buzzer_sound_on();
			fm_clear_flag(FLAG_BUZZER_ON);
			fm_set_flag_with_delay(FLAG_BUZZER_OFF, TIME_SOUND_ACTIVE);
		}
		if (fm_is_flag_set(FLAG_BUZZER_OFF))
		{
			buzzer_sound_off();
			fm_clear_flag(FLAG_BUZZER_OFF);
			fm_set_flag_with_delay(FLAG_BUZZER_ON, time_sound_frequency[time_sound_index]);
		}
		if (fm_is_flag_set(FLAG_BUZZER_CHANGE_FREQUENCY))
		{
			time_sound_index = 1;
			fm_clear_flag(FLAG_BUZZER_CHANGE_FREQUENCY);
		}
	}
}


static void app_yelow_blink(void)
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

void app_change_mode(void)
{
	switch (trafic_light_mode)
	{		
		case red:
		{
			light_set_color_state(auto_trafic_light, light_all, light_off);
			light_set_color_state(auto_trafic_light, light_red, light_on);
			fm_set_flag_with_delay(FLAG_CHANGE_MODE, times_trafic_light[trafic_light_mode]);
			
			/* Config fo pedestrian trafic light*/
			pedestrian_trafic_light_mode = pedestrian_green;
			fm_set_flag_with_delay(FLAG_PEDESTRIAN_CHANGE_MODE, TIME_STOP_ALL_TRAFIC);
			
			/* Next step parameters */
			trafic_light_mode = red_yelow;
			break;
		}
		case red_yelow:
		{
			light_set_color_state(auto_trafic_light, light_yelow, light_on);
			fm_set_flag_with_delay(FLAG_CHANGE_MODE, times_trafic_light[trafic_light_mode]);

			/* Next step parameters */
			trafic_light_mode = green;
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
			trafic_light_mode = green_blink;
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
				trafic_light_mode = yelow;
				fm_set_flag(FLAG_CHANGE_MODE);
			}
			else
			{
				fm_set_flag_with_delay(FLAG_CHANGE_MODE, 500);
			}
			break;
		}
		case yelow:
		{
			light_set_color_state(auto_trafic_light, light_green, light_off);
			light_set_color_state(auto_trafic_light, light_yelow, light_on);
			fm_set_flag_with_delay(FLAG_CHANGE_MODE, times_trafic_light[trafic_light_mode]);

			/* Next step parameters */
			trafic_light_mode = red;
			break;
		}
		default:
			break;
	}
}
void app_pedestrian_change_mode(void)
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
			fm_set_flag_with_delay(FLAG_PEDESTRIAN_CHANGE_MODE, time);
			
			fm_set_flag(FLAG_BUZZER_ON);
			time_sound_index = 0;
			fm_set_flag_with_delay(FLAG_BUZZER_CHANGE_FREQUENCY, time - 4000);
			
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
				fm_set_flag(FLAG_PEDESTRIAN_CHANGE_MODE);
				
				fm_clear_flag(FLAG_BUZZER_ON);
				fm_clear_flag(FLAG_BUZZER_OFF);
				fm_clear_flag_with_delay(FLAG_BUZZER_ON);
				fm_clear_flag_with_delay(FLAG_BUZZER_OFF);
            }
			else
            {
                fm_set_flag_with_delay(FLAG_PEDESTRIAN_CHANGE_MODE, 500);
				time_sound_index = 2;
            }
			break;
		}
	}	
}