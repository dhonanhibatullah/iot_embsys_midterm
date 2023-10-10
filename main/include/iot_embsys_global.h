#ifdef __DISPLAY_H__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/queue.h>
    #include <freertos/semphr.h>
    #include "ssd1306.h"
#endif

#ifdef __KEYPAD_H__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <driver/gpio.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/queue.h>
    #include <freertos/semphr.h>
#endif

#ifdef __GATE_H__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <esp_timer.h>
    #include <rom/ets_sys.h>
    #include <driver/gpio.h>
    #include <driver/ledc.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/queue.h>
    #include <freertos/semphr.h>
#endif

#ifdef __UART_H__
#endif