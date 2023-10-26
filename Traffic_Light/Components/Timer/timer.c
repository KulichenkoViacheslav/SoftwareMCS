#include "timer.h"
#include "tim.h"
#include <string.h>

#define TIMERES_CONFIG_NUMBER 5

static timer_cb timeout_ring;
static uint32_t tim_channels[TIMER_CHANNELS_MAX] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};
static uint32_t tim_active_channels[TIMER_CHANNELS_MAX] = {HAL_TIM_ACTIVE_CHANNEL_1, HAL_TIM_ACTIVE_CHANNEL_2, \
														   HAL_TIM_ACTIVE_CHANNEL_3, HAL_TIM_ACTIVE_CHANNEL_4};
static timer_s_t timer_context[TIMERES_CONFIG_NUMBER];

void timer_init(void)
{
	memset(&timer_context, 0, TIMERES_CONFIG_NUMBER * sizeof(timer_s_t));
}

void timer_set_config(timer_s_t * timer)
{
	for (uint8_t i = 0; i < TIMERES_CONFIG_NUMBER; i ++)
	{
		if (timer_context[i].id == TIMER_NONE)
		{
			if (timer->id == TIMER_5)
			{
				timer->htim = (uint32_t)&htim5;
				memcpy(&timer_context[i], timer, sizeof(timer_s_t));
				timer_update(timer);
				break;
			}
			if (timer->id == TIMER_6)
			{
				timer->htim = (uint32_t)&htim6;
				memcpy(&timer_context[i], timer, sizeof(timer_s_t));
				timer_update(timer);
				break;
			}
		}
	}
}

void timer_update(timer_s_t * timer)
{
	__HAL_TIM_SET_AUTORELOAD((TIM_HandleTypeDef *)timer->htim, timer->period);
	for (uint8_t i = 0; i < TIMER_CHANNELS_MAX; i ++)
	{
		if (timer->channel_cb[i] != NULL)
		{
			__HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)timer->htim, tim_channels[i], timer->channel_var[i]);
		}
	}
}

void timer_start(timer_s_t * timer)
{
	HAL_TIM_Base_Start_IT((TIM_HandleTypeDef *)timer->htim);
	for (uint8_t i = 0; i < TIMER_CHANNELS_MAX; i ++)
	{
		if (timer->channel_cb[i] != NULL)
		{
			HAL_TIM_PWM_Start_IT((TIM_HandleTypeDef *)timer->htim, tim_channels[i]);
		}
	}
}

void timer_stop(timer_s_t * timer)
{
	HAL_TIM_Base_Stop_IT((TIM_HandleTypeDef *)timer->htim);
	for (uint8_t i = 0; i < TIMER_CHANNELS_MAX; i ++)
	{
		if (timer->channel_cb[i] != NULL)
		{
			HAL_TIM_PWM_Stop_IT((TIM_HandleTypeDef *)timer->htim, tim_channels[i]);
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	for (uint8_t i = 0; i < TIMERES_CONFIG_NUMBER; i ++)
	{
		if (htim == (TIM_HandleTypeDef *)timer_context[i].htim)
		{
			if (timer_context[i].update_cb != NULL)
			{
				timer_context[i].update_cb();
			}
		}
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	for (uint8_t i = 0; i < TIMERES_CONFIG_NUMBER; i ++)
	{
		if (htim == (TIM_HandleTypeDef *)timer_context[i].htim)
		{
			for (uint8_t index = 0; index < TIMER_CHANNELS_MAX; index ++)
			{
				if (htim->Channel == tim_active_channels[index])
				{
					if (timer_context[i].channel_cb[index] != NULL)
					{
						timer_context[i].channel_cb[index]();
					}
				}
			}
		}
	}
}

void timer_empty_cb(void)
{
}