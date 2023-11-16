#include "debuger.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "usart.h"
#include "flag_machine.h"
#include "clock.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

#define BUFFER_SIZE 32

uint8_t input_buffer[BUFFER_SIZE] = {0};
uint8_t input_index = 0;
uint8_t str_time[10] = {0};

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t name[BUFFER_SIZE];
    uint8_t name_size;
    uint8_t value[BUFFER_SIZE];
    uint8_t value_size;
}command_s_t;

/* Format input message:
    #     - Control symbol
    name  - Name command
    :     - Separate symbol
    value - Value of parameter
    '/r'  - Last symbol
*/

static    command_s_t command = {0};
static bool debuger_convert_string_to_time(uint8_t * str, clock_time_s_t * time);

void debuger_init(void)
{
    HAL_UART_Receive_IT(&huart2, &input_buffer[input_index], 1);
}

void debuger_execute_command(void)
{
    /* Check start symbol in message */
    if (command.buffer[0] == '#')
    {
        /* Parser command */
        bool separator = false;
        uint8_t i = 0;

        for (i = 1; i < BUFFER_SIZE; i ++)
        {
            if (command.buffer[i] == '\r')
            {
                break;
            }
            if (separator == false)
            {
                if (command.buffer[i] != ':')
                {
                    /* Select Name of command */
                    command.name[command.name_size] = command.buffer[i];
                    command.name_size ++;
                }
                else
                {
                    separator = true;
                }
            }
            else
            {
                /* Select value of command */
                command.value[command.value_size] = command.buffer[i];
                command.value_size ++;
            }
        }
        
        /* Activate command */
        if (memcmp(command.name, "set time", command.name_size) == 0)
        {
            clock_data_time_s_t new_data_time = {0};
            clock_get_data_time(&new_data_time);
            if (debuger_convert_string_to_time(command.value, &new_data_time.time))
            {
                clock_set_data_time(&new_data_time);
            }
            else
            {
                printf("> Incorrect format of Value\r\n");
            }
        }
        else if (memcmp(command.name, "set state", command.name_size) == 0)
        {
            fm_clear_all_flags();
            if (command.value[0] == '0')
            {
                fm_set_flag(FLAG_TRAFIC_LIGHT_DISABLE);
            }
            if (command.value[0] == '1')
            {
                fm_set_flag(FLAG_TRAFIC_LIGHT_WARNING);
            }
            if (command.value[0] == '2')
            {
                fm_set_flag(FLAG_TRAFIC_LIGHT_ENABLE);
            }
        }
        else
        {
            printf("> No Matching Command Found\r\n");
        }
    }
    else
    {
        printf("> Incorrect format of Command\r\n");
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Check last symbol in received message */
    if (input_buffer[input_index] != '\r')
    {
        input_index ++;
        if (input_index == BUFFER_SIZE)
        {
            input_index = 0;
        }
    }
    /* Whole message was received */
    else
    {
        memset(&command, 0, sizeof(command_s_t));
        memcpy(command.buffer, input_buffer, input_index);
        fm_set_flag(FLAG_DEBUGER_RECEIVED_COMMAND);
        input_index = 0;
        printf("\r\n");
    }

    /* Restart Receiver */
    HAL_UART_Receive_IT(&huart2, &input_buffer[input_index], 1);
}

static bool debuger_convert_time_to_string(uint8_t * str, clock_time_s_t * time)
{
    bool status = true;
    str[0] = (time->hours / 10 + 0x30);
    str[1] = (time->hours % 10 + 0x30);
    str[2] = ':';
    str[3] = (time->minutes / 10 + 0x30);
    str[4] = (time->minutes % 10 + 0x30);
    str[5] = ':';
    str[6] = (time->seconds / 10 + 0x30);
    str[7] = (time->seconds % 10 + 0x30);
    return status;
}

uint8_t * debuger_get_curr_time(void)
{
    clock_data_time_s_t curr_date_time;
    clock_get_data_time(&curr_date_time);
    debuger_convert_time_to_string(str_time, &curr_date_time.time);
    return str_time;
}


static bool debuger_convert_string_to_time(uint8_t * str, clock_time_s_t * time)
{
    bool status = true;
    if ((str[0] < 0x30) || (str[0] > 0x31) ||
        (str[1] < 0x30) || (str[1] > 0x39) ||
        (str[3] < 0x30) || (str[3] > 0x35) ||
        (str[4] < 0x30) || (str[4] > 0x39) ||
        (str[6] < 0x30) || (str[6] > 0x35) ||
        (str[7] < 0x30) || (str[7] > 0x39))
    {
        status = false;
    }
    else
    {
        time->hours   = (str[0] - 0x30) * 10 + (str[1] - 0x30);
        time->minutes = (str[3] - 0x30) * 10 + (str[4] - 0x30);
        time->seconds = (str[5] - 0x30) * 10 + (str[6] - 0x30);
    }
    return status;
}
