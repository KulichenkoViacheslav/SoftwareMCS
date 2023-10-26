#include "flag_machine.h"
#include "timer.h"

typedef enum
{
	off = 0,
	on  = 1
}fm_flag_delay_state_e_t;

typedef struct
{
	fm_flag_delay_state_e_t state;
	fm_flags_e_t            flag;
	uint32_t                time;
}fm_flag_delay_s_t;

#define FLAG_DELAY_CONTEXT_SIZE 10

static fm_flag_delay_s_t fm_flag_delay_context[FLAG_DELAY_CONTEXT_SIZE] = {0};
static timer_s_t fm_timer = {0};
static uint32_t fm_core = 0;
void fm_timer_delay(void);

void fm_init(void)
{
	fm_core = 0;
	
	fm_timer.id        = TIMER_6;
	fm_timer.period    = 1000;
	fm_timer.update_cb = fm_timer_delay;
	
	timer_set_config(&fm_timer);
	timer_start(&fm_timer);
}

bool fm_is_flag_set(fm_flags_e_t flag)
{
	return (fm_core & (1 << flag));
}

void fm_set_flag(fm_flags_e_t flag)
{
	fm_core |= (1 << flag);
}

void fm_clear_flag(fm_flags_e_t flag)
{
	fm_core &= (~(1 << flag));
}

// time in ms
void fm_set_flag_with_delay(fm_flags_e_t flag, uint32_t time)
{
	for (uint8_t i = 0; i < FLAG_DELAY_CONTEXT_SIZE; i ++)
	{
		if (fm_flag_delay_context[i].state == off)
		{
			fm_flag_delay_context[i].state = on;
			fm_flag_delay_context[i].time = time;
			fm_flag_delay_context[i].flag = flag;
			break;
		}
	}
}

void fm_clear_flag_with_delay(fm_flags_e_t flag)
{
	for (uint8_t i = 0; i < FLAG_DELAY_CONTEXT_SIZE; i ++)
	{
		if (fm_flag_delay_context[i].flag == flag)
		{
			fm_flag_delay_context[i].state = off;
			fm_flag_delay_context[i].time = 0;
			fm_flag_delay_context[i].flag = 0;
			
			fm_clear_flag(flag);
			break;
		}
	}
}

void fm_timer_delay(void)
{
	for (uint8_t i = 0; i < FLAG_DELAY_CONTEXT_SIZE; i ++)
	{
		if (fm_flag_delay_context[i].state == on)
		{
			fm_flag_delay_context[i].time --;
			if (fm_flag_delay_context[i].time == 0)
			{
				fm_set_flag(fm_flag_delay_context[i].flag);
				fm_flag_delay_context[i].state = off;
			}
		}
	}
}