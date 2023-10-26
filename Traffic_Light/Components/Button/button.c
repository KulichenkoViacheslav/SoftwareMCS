#include "button.h"
#include <stdint.h>
#include "gpio.h"
#include "stdbool.h"
#include "flag_machine.h"

GPIO_PinState pin_previous_state = GPIO_PIN_RESET;

void button_init(void)
{
	pin_previous_state = GPIO_PIN_RESET;
}

void button_check(void)
{
	if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET)
	{
		if (pin_previous_state == GPIO_PIN_RESET)
		{
			fm_set_flag(FLAG_BUTTON_PRESSED);
			pin_previous_state = GPIO_PIN_SET;
		}
	}
	else
	{
		pin_previous_state = GPIO_PIN_RESET;
	}
}