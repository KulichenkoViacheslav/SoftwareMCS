#include "button.h"
#include <stdint.h>
#include "gpio.h"

static button_pressed_cb button_pressed;

void button_init(button_pressed_cb p_button_pressed_cb)
{
	button_pressed = p_button_pressed_cb;
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if(pin == BUTTON_Pin)
	{
		button_pressed();
	}
}