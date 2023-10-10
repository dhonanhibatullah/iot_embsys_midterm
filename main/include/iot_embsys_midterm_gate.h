#ifndef __GATE_H__
#define __GATE_H__

#include "iot_embsys_global.h"

#define IOTEM_GATE_SERVO_SIGNAL     5
#define IOTEM_GATE_PROX_TRIG        32
#define IOTEM_GATE_PROX_ECHO        35
#define IOTEM_GATE_PWM_CHANNEL      LEDC_CHANNEL_0
#define IOTEM_GATE_TIMER_CHANNEL    LEDC_TIMER_0
#define IOTEM_GATE_PWM_FREQ         1000
#define IOTEM_GATE_PWM_RESOLUTION   LEDC_TIMER_10_BIT   

#define IOTEM_GATE_PWM_OPEN     1023
#define IOTEM_GATE_PWM_CLOSE    0

#define IOTEM_GATE_WAITING  0
#define IOTEM_GATE_PASSED   1

#define IOTEM_GATE_TRIG_ON_US       10
#define IOTEM_GATE_ECHO_TIMEOUT_US  3000
#define IOTEM_GATE_TASK_DELAY_MS    1500


extern SemaphoreHandle_t    pass_mutex;
extern uint8_t              pass_status;


void iotem_gate_begin();
void iotem_gate_open();
void iotem_gate_close();
float iotem_gate_get_prox_dist();
void iotem_gate_task(void* pvParameters);


#endif