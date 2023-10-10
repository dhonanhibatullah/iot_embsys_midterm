#include "include/iot_embsys_midterm_uart.h"



void iotem_uart_begin() {

    /* Initiate UART for serial communication */
    uart_config_t uart_config = {
        .baud_rate = IOTEM_UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 2048, 2048, 10, NULL, 0);
}