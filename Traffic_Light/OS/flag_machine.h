#ifndef FLAG_MACHINE_H
#define FLAG_MACHINE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
	FLAG_BUTTON_PRESSED = 0,
	FLAG_BUTTON_CHECK,
	FLAG_BUTTON_LOCK,
	FLAG_BUTTON_UNLOCK,
	FLAG_YELOW_BLINK,
	FLAG_CHANGE_MODE,
	FLAG_TRAFIC_LIGHT_DISABLE,
	FLAG_TRAFIC_LIGHT_ENABLE,
	FLAG_TRAFIC_LIGHT_WAITING,
}fm_flags_e_t;

void fm_init(void);
bool fm_is_flag_set(fm_flags_e_t flag);
void fm_set_flag(fm_flags_e_t flag);
void fm_clear_flag(fm_flags_e_t flag);
void fm_set_flag_with_delay(fm_flags_e_t flag, uint32_t time);
void fm_clear_flag_with_delay(fm_flags_e_t flag);

#endif