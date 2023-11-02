#include "buzzer.h"
#include "main.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

void buzzer_init(void)
{
	buzzer_sound_off();
}

void buzzer_sound_on(void)
{
  HAL_GPIO_WritePin(PEDESTRIAN_SOUND_GPIO_Port, PEDESTRIAN_SOUND_Pin, GPIO_PIN_RESET);
}

void buzzer_sound_off(void)
{
  HAL_GPIO_WritePin(PEDESTRIAN_SOUND_GPIO_Port, PEDESTRIAN_SOUND_Pin, GPIO_PIN_SET);
    
}