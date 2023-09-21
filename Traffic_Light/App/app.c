#include "app.h"
#include "light.h"
#include "timer.h"
#include "stm32f4xx_hal.h"

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
	yelow_blink = 0,
	red,
	red_yelow,
	green,
	green_blink,
	yelow,
}trafic_light_mode_e_t;

//Red - 30 s; Red + Yelow - 3 s; Green - 15 s; Green blink - 4 s; Yelow - 3 s; blink - 0.5/0.5 s

static trafic_light_mode_e_t trafic_light_mode = yelow_blink;
static uint8_t trafic_light_blink = 0;

static void app_change_mode(void);

void app_init(void)
{
	timer_init();
	light_init();
	
	timer_start(500, app_change_mode);
}	

void app_run(void)
{
	trafic_light_mode = red;
	while(1)
	{

	}
}

static void app_change_mode(void)
{
	switch (trafic_light_mode)
	{
		case yelow_blink:
		{
			if  (trafic_light_blink == 0)
			{
				light_set_color_state(light_yelow, light_on);
				timer_start(500, app_change_mode);
				trafic_light_blink = 1;
			}
			else
			{
				light_set_color_state(light_yelow, light_off);
				timer_start(500, app_change_mode);
				trafic_light_blink = 0;
			}
			break;
		}
		case red:
		{
			light_set_color_state(light_all, light_off);
			light_set_color_state(light_red, light_on);
			timer_start(30000, app_change_mode);
			trafic_light_mode = red_yelow;
			break;
		}
		case red_yelow:
		{
			light_set_color_state(light_yelow, light_on);
			timer_start(3000, app_change_mode);
			trafic_light_mode = green;
			break;
		}
		case green:
		{
			light_set_color_state(light_all, light_off);
  		light_set_color_state(light_green, light_on);
			timer_start(15000, app_change_mode);
			trafic_light_mode = green_blink;
			trafic_light_blink = 8;
			break;
		}
		case green_blink:
		{
			if ((trafic_light_blink % 2) == 0)
			{
				light_set_color_state(light_green, light_off);
				timer_start(500, app_change_mode);
			}
			else
			{
				light_set_color_state(light_green, light_on);
				timer_start(500, app_change_mode);
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
			light_set_color_state(light_green, light_off);
			light_set_color_state(light_yelow, light_on);
			timer_start(3000, app_change_mode);
			trafic_light_mode = red;
			break;
		}
	}
}

//		light_set_color_state(light_red, light_on);
//		HAL_Delay(30000);
//		
//		light_set_color_state(light_yelow, light_on);
//		HAL_Delay(3000);
//		
//		light_set_color_state(light_all, light_off);
//		light_set_color_state(light_green, light_on);
//		HAL_Delay(15000);
//		
//		light_set_color_state(light_green, light_off);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_on);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_off);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_on);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_off);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_on);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_off);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_on);
//		HAL_Delay(500);
//		light_set_color_state(light_green, light_off);
//		
//		light_set_color_state(light_yelow, light_on);
//		HAL_Delay(3000);
