#include "app.h"
#include "light.h"
#include "timer.h"
#include "button.h"
#include "stm32f4xx_hal.h"
#include "flag_machine.h"
#include "led_display_auto.h"

// Variants
/*
1. Red
2. Red + Yelow
3. Green
4. Green blink
5. Yelow
*/

typedef enum
{
	red = 0,
	red_yelow,
	green,
	green_blink,
	yelow,
}trafic_light_mode_e_t;

//Red - 30 s; Red + Yelow - 3 s; Green - 15 s; Green blink - 4 s; Yelow - 3 s; blink - 0.5/0.5 s

static trafic_light_mode_e_t trafic_light_mode = red;
static uint8_t trafic_light_blink = 0;
static bool blinker_switch = false;

static void app_change_mode(void);
static void app_yelow_blink(void);

void app_init(void)
{
	timer_init();
	fm_init();
	light_init();
	led_display_auto_init();

	light_set_color_state(auto_trafic_light, light_all, light_off);
	light_set_color_state(pedestrian_trafic_light, light_all, light_off);
	
	fm_set_flag_with_delay(FLAG_YELOW_BLINK, 500);
}	

void app_run(void)
{
	while (1)
	{
		if (fm_is_flag_set(FLAG_BUTTON_PRESSED))
		{
			fm_clear_flag(FLAG_BUTTON_PRESSED);
		}
		if (fm_is_flag_set(FLAG_BUTTON_CHECK))
		{
			
		}
		if (fm_is_flag_set(FLAG_YELOW_BLINK))
		{
			app_yelow_blink();
			fm_clear_flag(FLAG_YELOW_BLINK);
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
	fm_set_flag_with_delay(FLAG_YELOW_BLINK, 500);
}

void app_change_mode(void)
{
	switch (trafic_light_mode)
	{		
		case red:
		{
			light_set_color_state(auto_trafic_light, light_all, light_off);
			light_set_color_state(auto_trafic_light, light_red, light_on);
			trafic_light_mode = red_yelow;
			break;
		}
		case red_yelow:
		{
			light_set_color_state(auto_trafic_light, light_yelow, light_on);
			trafic_light_mode = green;
			break;
		}
		case green:
		{			
			light_set_color_state(auto_trafic_light, light_all, light_off);
			light_set_color_state(auto_trafic_light, light_green, light_on);
			fm_set_flag(FLAG_BUTTON_CHECK);
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
				trafic_light_mode = yelow;
			}
			break;
		}
		case yelow:
		{
			light_set_color_state(auto_trafic_light, light_green, light_off);
			light_set_color_state(auto_trafic_light, light_yelow, light_on);
			trafic_light_mode = red;
			break;
		}
	}
}
