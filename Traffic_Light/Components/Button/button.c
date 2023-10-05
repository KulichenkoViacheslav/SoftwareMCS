#include "button.h"
#include <stdint.h>
#include "gpio.h"
#include "stdbool.h"

static button_pressed_cb button_pressed;
static bool button_is_pressed = false;
static bool button_is_enabled = true;

void button_init(button_pressed_cb p_button_pressed_cb)
{
	button_pressed = p_button_pressed_cb;
}



void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if(pin == BUTTON_Pin)
	{
		if (button_is_enabled == true)
		{
			button_pressed();
			button_is_enabled = false;
		}
		else 
		{
			button_is_pressed = true;
		}
	}
}

void button_check(void)
{
	if (button_is_pressed == true)
	{
		button_pressed();
		button_is_pressed = false;
		button_is_enabled = false;
	}
	else
	{		
		button_is_enabled = true;
	}
}