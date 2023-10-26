#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*timer_cb)(void);

typedef enum
{
	TIMER_NONE = 0,
	TIMER_5    = 5,
	TIMER_6    = 6,
}timer_id_e_t;

#define TIMER_CHANNELS_MAX 4

typedef struct
{
	/* Controls */
	timer_id_e_t id;
	/* Return parameters if Init success */
	uint32_t htim;
	/* Parameters */
	uint16_t period;
	uint16_t channel_var[TIMER_CHANNELS_MAX];
	/* Callbacks*/
	timer_cb update_cb;
	timer_cb channel_cb[TIMER_CHANNELS_MAX];
}timer_s_t;

void timer_init(void);
void timer_set_config(timer_s_t * timer);
void timer_start(timer_s_t * timer);
void timer_update(timer_s_t * timer);
bool timer_deinit(timer_s_t * timer);
void timer_stop(timer_s_t * timer);
/* Uses for active callback without intterapt function */
void timer_empty_cb(void);

#endif