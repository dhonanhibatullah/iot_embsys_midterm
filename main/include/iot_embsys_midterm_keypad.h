#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "iot_embsys_global.h"

#define IOTEM_KEYPAD_ROW    4
#define IOTEM_KEYPAD_COL    4
#define IOTEM_KEYPAD_R1     14
#define IOTEM_KEYPAD_R2     27
#define IOTEM_KEYPAD_R3     26
#define IOTEM_KEYPAD_R4     25
#define IOTEM_KEYPAD_C1     33
#define IOTEM_KEYPAD_C2     34
#define IOTEM_KEYPAD_C3     39
#define IOTEM_KEYPAD_C4     36

#define IOTEM_KEYPAD_1      0
#define IOTEM_KEYPAD_2      1
#define IOTEM_KEYPAD_3      2
#define IOTEM_KEYPAD_A      3
#define IOTEM_KEYPAD_4      4
#define IOTEM_KEYPAD_5      5
#define IOTEM_KEYPAD_6      6
#define IOTEM_KEYPAD_B      7
#define IOTEM_KEYPAD_7      8
#define IOTEM_KEYPAD_8      9
#define IOTEM_KEYPAD_9      10
#define IOTEM_KEYPAD_C      11
#define IOTEM_KEYPAD_STAR   12
#define IOTEM_KEYPAD_0      13
#define IOTEM_KEYPAD_HASH   14
#define IOTEM_KEYPAD_D      15
#define IOTEM_KEYPAD_NULL   99

#define IOTEM_KEYPAD_PASSWORD_MAX_LEN       16
#define IOTEM_KEYPAD_PASSWORD_QUEUE_SIZE    16

#define IOTEM_KEYPAD_AUTH_IDLE      0
#define IOTEM_KEYPAD_AUTH_SUCCESS   1
#define IOTEM_KEYPAD_AUTH_FAILED    2

#define IOTEM_KEYPAD_TASK_DELAY_MS  175


extern QueueHandle_t        password_queue;
extern SemaphoreHandle_t    auth_mutex;
extern uint8_t              auth_status;
extern char                 password[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1];


void iotem_keypad_begin();
uint8_t iotem_keypad_read();
bool iotem_keypad_auth(const char* input, const char* password);
void iotem_keypad_task(void* pvParameters);


#endif