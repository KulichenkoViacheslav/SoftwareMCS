#include "buzzer.h"
#include "main.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

void buzzer_init(void)
{
	buzzer_set(sound_off);
}

void buzzer_set(buzzer_sound_state_e_t state)
{
	if (state == sound_on)
	{		
		HAL_GPIO_WritePin(PEDESTRIAN_SOUND_GPIO_Port, PEDESTRIAN_SOUND_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(PEDESTRIAN_SOUND_GPIO_Port, PEDESTRIAN_SOUND_Pin, GPIO_PIN_SET);
	}
}
