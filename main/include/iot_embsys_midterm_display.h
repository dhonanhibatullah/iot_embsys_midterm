#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "iot_embsys_global.h"

#define IOTEM_SDA_GPIO          21
#define IOTEM_SCL_GPIO          22
#define IOTEM_RESET_GPIO        -1

#define IOTEM_SSD1306_WIDTH     128
#define IOTEM_SSD1306_HEIGHT    64
#define IOTEM_SSD1306_CONTRAST  0xff

#define IOTEM_DISPLAY_SPLASH_SCREEN     0
#define IOTEM_DISPLAY_PASSWORD          1
#define IOTEM_DISPLAY_ACCESS_GRANTED    2
#define IOTEM_DISPLAY_ACCESS_DENIED     3
#define IOTEM_DISPLAY_CHANGE_PASSWORD   4

#define IOTEM_DISPLAY_TASK_DELAY_MS     50
#define IOTEM_DISPLAY_SPLASH_SCREEN_MS  2000


/* SSD1306_t somehow need to be defined as a global variable to work in a task. */
extern SSD1306_t    display;


void iotem_display_begin(SSD1306_t* _display);
void iotem_display_task(void* pvParameters);


#endif