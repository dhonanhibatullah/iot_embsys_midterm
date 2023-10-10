#ifndef __GATE_H__
#define __GATE_H__

#include "iot_embsys_global.h"

#define IOTEM_GATE_SERVO_SIGNAL     5
#define IOTEM_GATE_PROX_TRIG        32
#define IOTEM_GATE_PROX_ECHO        35
#define IOTEM_GATE_PWM_CHANNEL      LEDC_CHANNEL_0
#define IOTEM_GATE_TIMER_CHANNEL    LEDC_TIMER_0
#define IOTEM_GATE_PWM_FREQ         50
#define IOTEM_GATE_PWM_RESOLUTION   LEDC_TIMER_13_BIT   

#define IOTEM_GATE_PWM_OPEN     650
#define IOTEM_GATE_PWM_CLOSE    270

#define IOTEM_GATE_CLOSED       0
#define IOTEM_GATE_OPENED       1
#define IOTEM_GATE_NOT_PASSED   2
#define IOTEM_GATE_PASSED       3

#define IOTEM_GATE_TRIG_ON_US       50
#define IOTEM_GATE_ECHO_TIMEOUT_US  3000
#define IOTEM_GATE_TASK_DELAY_MS    300
#define IOTEM_GATE_PASS_DELAY_MS    80


extern SemaphoreHandle_t    gate_mutex;
extern uint8_t              gate_status;


void iotem_gate_begin();
void iotem_gate_open();
void iotem_gate_close();
uint8_t iotem_gate_get_pass();
void iotem_gate_task(void* pvParameters);


#endif