#include "light.h"
#include "gpio.h"
#include "timer.h"

#define BRIGHTNESS_MAX 100

typedef struct
{
	uint8_t red;
	uint8_t yelow;
	uint8_t green;
}light_brightness_s_t;

typedef struct
{
	light_state_e_t red_auto;
	light_state_e_t yelow_auto;
	light_state_e_t green_auto;
	light_state_e_t red_pedestrian;
	light_state_e_t green_pedestrian;
}light_state_s_t;

static light_brightness_s_t light_brightness;
static timer_s_t timer_light_brightness;
static light_state_s_t light_state;

void light_pwm_update(void);
void light_pwm_red(void);
void light_pwm_yelow(void);
void light_pwm_green(void);


void light_init(void)
{
	light_set_color_state(auto_trafic_light, light_all, light_off);
	light_set_color_state(pedestrian_trafic_light, light_all, light_off);
	light_set_color_state(pedestrian_trafic_light, light_green, light_on);
	light_brightness.red = 50;
	light_brightness.green = 5;
	light_brightness.yelow = 5;
	
	timer_light_brightness.id 		      = TIMER_5;
	timer_light_brightness.period         = 100;
	timer_light_brightness.channel_var[0] = light_brightness.red;
	timer_light_brightness.channel_var[1] = light_brightness.yelow;
	timer_light_brightness.channel_var[2] = light_brightness.green;
	
	timer_light_brightness.update_cb     = light_pwm_update;
	timer_light_brightness.channel_cb[0] = light_pwm_red;
	timer_light_brightness.channel_cb[1] = light_pwm_yelow;
	timer_light_brightness.channel_cb[2] = light_pwm_green;
	
	timer_set_config(&timer_light_brightness);
	timer_start(&timer_light_brightness);
}	

void light_set_color_state(trafic_light_e_t type_trafic_light, light_color_e_t color, light_state_e_t state)
{
	switch (type_trafic_light)
	{
		case auto_trafic_light:
		{
			switch (color)
			{
				case light_red:
				{
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, (GPIO_PinState)state);
					light_state.red_auto = state;
					break;
				}
				case light_yelow:
				{
					HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, (GPIO_PinState)state);
					light_state.yelow_auto = state;
					break;
				}
				case light_green:
				{
					HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, (GPIO_PinState)state);
					light_state.green_auto = state;
					break;
				}
				case light_all:
				{
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, (GPIO_PinState)state);
					HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, (GPIO_PinState)state);
					HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, (GPIO_PinState)state);
					light_state.red_auto = state;
					light_state.yelow_auto = state;
					light_state.green_auto = state;
					break;
				}
				default:
				{
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, GPIO_PIN_SET);
					light_state.red_auto = light_off;
					light_state.yelow_auto = light_off;
					light_state.green_auto = light_off;
					break;
				}
			}
			break;
		}
		case pedestrian_trafic_light:
		{
			switch (color)
			{
				case light_red:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, (GPIO_PinState)state);
					light_state.red_pedestrian = state;
					break;
				}
				case light_green:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, (GPIO_PinState)state);
					light_state.green_pedestrian = state;
					break;
				}
				case light_all:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, (GPIO_PinState)state);
					HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, (GPIO_PinState)state);
					light_state.red_pedestrian = state;
					light_state.green_pedestrian = state;
					break;
				}
				default:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, GPIO_PIN_SET);
					light_state.red_pedestrian = light_off;
					light_state.green_pedestrian = light_off;
					break;
				}
			}
			break;
		}
		default:
		{
			/* For AUTO-MOTO */
			HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, GPIO_PIN_SET);
			light_state.red_auto = light_off;
			light_state.yelow_auto = light_off;
			light_state.green_auto = light_off;

			/* FOR PEDESTRIAN */
			HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, GPIO_PIN_SET);
			light_state.red_pedestrian = light_off;
			light_state.green_pedestrian = light_off;
			break;
		}
	}
}

/* Regulation PWM */
//-----------------------------------------------------------------------------
void light_pwm_update(void)
{
	if (light_state.red_auto == light_on)
	{
		HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, GPIO_PIN_RESET);
	}
	if (light_state.yelow_auto == light_on)
	{
		HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, GPIO_PIN_RESET);
	}
	if (light_state.green_auto == light_on)
	{
		 HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, GPIO_PIN_RESET);
	}
	if (light_state.red_pedestrian == light_on)
	{
		HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, GPIO_PIN_RESET);
	}
	if (light_state.green_pedestrian == light_on)
	{
		HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, GPIO_PIN_RESET);
	}
}

void light_pwm_red(void)
{
    HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, GPIO_PIN_SET);
}

void light_pwm_yelow(void)
{
	HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, GPIO_PIN_SET);
}

void light_pwm_green(void)
{
	HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, GPIO_PIN_SET);
}
//-----------------------------------------------------------------------------


					
					