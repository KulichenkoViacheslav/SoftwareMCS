#include "debuger.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void debuger_init(void)
{
	//printf("Init compleated");
}
