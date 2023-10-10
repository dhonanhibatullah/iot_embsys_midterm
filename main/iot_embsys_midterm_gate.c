#include "include/iot_embsys_midterm_gate.h"




void iotem_gate_begin() {
    
    /* Configure LEDC channel */
    ledc_timer_config_t timer_conf = {
        .speed_mode         = LEDC_HIGH_SPEED_MODE,
        .duty_resolution    = IOTEM_GATE_PWM_RESOLUTION,
        .timer_num          = IOTEM_GATE_TIMER_CHANNEL,
        .freq_hz            = IOTEM_GATE_PWM_FREQ,
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t ledc_conf = {
        .channel    = IOTEM_GATE_PWM_CHANNEL,
        .duty       = 0,
        .gpio_num   = IOTEM_GATE_SERVO_SIGNAL,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel  = IOTEM_GATE_TIMER_CHANNEL,
    };
    ledc_channel_config(&ledc_conf);


    /* Configure GPIO */
    esp_rom_gpio_pad_select_gpio(IOTEM_GATE_PROX_ECHO);
    gpio_set_direction(IOTEM_GATE_PROX_ECHO, GPIO_MODE_INPUT);
    esp_rom_gpio_pad_select_gpio(IOTEM_GATE_PROX_TRIG);
    gpio_set_direction(IOTEM_GATE_PROX_TRIG, GPIO_MODE_OUTPUT);
    gpio_set_level(IOTEM_GATE_PROX_TRIG, 0);

    /* Close the gate */
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL, IOTEM_GATE_PWM_CLOSE);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL);
}




void iotem_gate_open() {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL, IOTEM_GATE_PWM_OPEN);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL);
}




void iotem_gate_close() {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL, IOTEM_GATE_PWM_CLOSE);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL);
}




uint8_t iotem_gate_get_pass() {
    static bool object_detect = false;
    uint8_t ret_val = IOTEM_GATE_NOT_PASSED;
    
    gpio_set_level(IOTEM_GATE_PROX_TRIG, 1);
    ets_delay_us(IOTEM_GATE_TRIG_ON_US);
    gpio_set_level(IOTEM_GATE_PROX_TRIG, 0);
    
    /* Check whether an object exist in front of ultrasonic sensor */
    uint32_t time_elapsed_us = 0;
    if(!object_detect) {
        while(time_elapsed_us < IOTEM_GATE_ECHO_TIMEOUT_US) {
            if(gpio_get_level(IOTEM_GATE_PROX_ECHO)) {
                object_detect = true;
                break;
            }
            ets_delay_us(10);
            time_elapsed_us += 10;
        }
    }

    /* If object disappear, return object passed*/
    else {
        bool object_still_exist = false;
        while(time_elapsed_us < IOTEM_GATE_ECHO_TIMEOUT_US) {
            if(gpio_get_level(IOTEM_GATE_PROX_ECHO)) {
                object_still_exist = true;
                break;
            }
        }
        if(!object_still_exist) {
            object_detect = false;
            ret_val = IOTEM_GATE_PASSED;
        }
    }

    return ret_val;
}