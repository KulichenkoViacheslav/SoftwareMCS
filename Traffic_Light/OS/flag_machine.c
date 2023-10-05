#include "flag_machine.h"
#include "stdint.h"

#include "button.h"

static uint32_t fm_core = 0;

void fm_init(void)
{
	button_init();
}

void fm_run(void)
{
	if (fm_core & (1 << BUTTON_PRESSED))
	{
		button_check();
		fm_clear_flag(BUTTON_PRESSED);
	}
	else if (fm_core & (1 << BUTTON_PRESSED)
	{
		
	}
}

void fm_set_flag(fm_flags_e_t flag)
{
	fm_core |= (1 << flag);
}

void fm_clear_flag(fm_flags_e_t flag)
{
	fm_core &= (~(1 << flag));
}