#ifndef BUZZER_H
#define BUZZER_H

typedef enum
{
	sound_off = 0,
	sound_on  = 1,
}buzzer_sound_state_e_t;

void buzzer_init(void);
void buzzer_set(buzzer_sound_state_e_t state);

#endif