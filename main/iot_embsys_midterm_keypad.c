#include "include/iot_embsys_midterm_keypad.h"




void iotem_keypad_begin() {

    /* GPIO configurations */
    gpio_config_t io_conf;
    io_conf.intr_type       = GPIO_INTR_DISABLE;
    io_conf.mode            = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en      = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;

    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_R1);
    gpio_config(&io_conf);
    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_R2);
    gpio_config(&io_conf);
    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_R3);
    gpio_config(&io_conf);
    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_R4);
    gpio_config(&io_conf);

    io_conf.mode            = GPIO_MODE_INPUT;
    io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;

    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_C1);
    gpio_config(&io_conf);
    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_C2);
    gpio_config(&io_conf);
    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_C3);
    gpio_config(&io_conf);
    io_conf.pin_bit_mask    = (1ULL << IOTEM_KEYPAD_C4);
    gpio_config(&io_conf);


    /* Initiate level */
    gpio_set_level(IOTEM_KEYPAD_R1, 0);
    gpio_set_level(IOTEM_KEYPAD_R2, 0);
    gpio_set_level(IOTEM_KEYPAD_R3, 0);
    gpio_set_level(IOTEM_KEYPAD_R4, 0);
}




uint8_t iotem_keypad_read() {

    /* Static variables */
    static uint8_t  rows[5] = {IOTEM_KEYPAD_R1, IOTEM_KEYPAD_R2, IOTEM_KEYPAD_R3, IOTEM_KEYPAD_R4},
                    cols[5] = {IOTEM_KEYPAD_C1, IOTEM_KEYPAD_C2, IOTEM_KEYPAD_C3, IOTEM_KEYPAD_C4};


    /* Turn HIGH all rows and read columns */
    bool    detected        = false;
    uint8_t pressed_button  = IOTEM_KEYPAD_NULL;
    for(uint8_t i = 0; i < IOTEM_KEYPAD_ROW; ++i) {
        gpio_set_level(rows[i], 1);
        
        for(uint8_t j = 0; j < IOTEM_KEYPAD_COL; ++j) {
            bool state     = (bool)gpio_get_level(cols[j]);
            
            if(state) {
                pressed_button = i*IOTEM_KEYPAD_ROW + j;
                detected = true;
                break;
            }
        }

        gpio_set_level(rows[i], 0);
        if(detected) break;
    }


    /* Return pressed button */
    return pressed_button;
}




bool iotem_keypad_auth(const char* input, const char* password) {
    bool ret_val = false;
    if(strcmp(input, password) == 0) {
        ret_val = true;
    }
    return ret_val;
}