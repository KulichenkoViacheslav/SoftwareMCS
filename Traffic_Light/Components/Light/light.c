#include "light.h"
#include "gpio.h"

void light_init(void)
{
	light_set_color_state(auto_trafic_light, light_all, light_off);
	light_set_color_state(pedestrian_trafic_light, light_all, light_off);
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
					break;
				}
				case light_yelow:
				{
					HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, (GPIO_PinState)state);
					break;
				}
				case light_green:
				{
					HAL_GPIO_WritePin(AUTO_GREEN_GPIO_Port, AUTO_GREEN_Pin, (GPIO_PinState)state);
					break;
				}
				case light_all:
				{
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, (GPIO_PinState)state);
					HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, (GPIO_PinState)state);
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, (GPIO_PinState)state);
					break;
				}
				default:
				{
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(AUTO_YELOW_GPIO_Port, AUTO_YELOW_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(AUTO_RED_GPIO_Port, AUTO_RED_Pin, GPIO_PIN_SET);
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
					break;
				}
				case light_green:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, (GPIO_PinState)state);
					break;
				}
				case light_all:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, (GPIO_PinState)state);
					HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, (GPIO_PinState)state);
					break;
				}
				default:
				{
					HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, GPIO_PIN_SET);
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

			/* FOR PEDESTRIAN */
			HAL_GPIO_WritePin(PEDESTRIAN_RED_GPIO_Port, PEDESTRIAN_RED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PEDESTRIAN_GREEN_GPIO_Port, PEDESTRIAN_GREEN_Pin, GPIO_PIN_SET);
			break;
		}
	}
}