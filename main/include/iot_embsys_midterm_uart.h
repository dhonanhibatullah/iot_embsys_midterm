#ifndef __UART_H__
#define __UART_H__

#include "iot_embsys_global.h"

#define IOTEM_UART_BAUDRATE         115200

#define IOTEM_UART_DELAY_MS         100
#define IOTEM_UART_TASK_DELAY_MS    100

#define IOTEM_PASSWORD_IDLE         0
#define IOTEM_PASSWORD_CHANGE       1

#define IOTEM_PASSWORD_CHANGE_NO    0
#define IOTEM_PASSWORD_CHANGE_YES   1


extern uint8_t password_change_req;


void iotem_uart_begin();
void iotem_uart_task(void* pvParameters);


#endif