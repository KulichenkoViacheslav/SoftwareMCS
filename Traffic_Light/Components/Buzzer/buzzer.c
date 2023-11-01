#include "buzzer.h"
#include "main.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"



void buzzer_init(void)
{

}

 void buzzer_start()
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  HAL_Delay(20000);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
     
}
  