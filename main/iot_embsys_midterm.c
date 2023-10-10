#include "include/iot_embsys_midterm_display.h"
#include "include/iot_embsys_midterm_keypad.h"
#include "include/iot_embsys_midterm_gate.h"
// #include "include/iot_embsys_midterm_uart.h"



void app_begin() {
    xTaskCreate(
        iotem_display_task,
        "Display Task",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        iotem_keypad_task,
        "Keypad Task",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        iotem_gate_task,
        "Gate Task",
        2048,
        NULL,
        1,
        NULL
    );
}



void app_main() {
    app_begin();
}