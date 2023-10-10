#include "include/iot_embsys_midterm_gate.h"




void iotem_gate_begin() {
    
    /* Configure LEDC channel */
    ledc_timer_config_t ledc_timer_conf = {
        .speed_mode         = LEDC_HIGH_SPEED_MODE,
        .duty_resolution    = IOTEM_GATE_PWM_RESOLUTION,
        .timer_num          = IOTEM_GATE_TIMER_CHANNEL,
        .freq_hz            = IOTEM_GATE_PWM_FREQ
    };
    ledc_timer_config(&ledc_timer_conf);

    ledc_channel_config_t ledc_channel_conf = {
        .channel    = IOTEM_GATE_PWM_CHANNEL,
        .duty       = 0,
        .gpio_num   = IOTEM_GATE_SERVO_SIGNAL,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel  = IOTEM_GATE_TIMER_CHANNEL
    };
    ledc_channel_config(&ledc_channel_conf);

    
    /* Configure GPIO */
    esp_rom_gpio_pad_select_gpio(IOTEM_GATE_PROX_ECHO);
    gpio_set_direction(IOTEM_GATE_PROX_ECHO, GPIO_MODE_INPUT);
    esp_rom_gpio_pad_select_gpio(IOTEM_GATE_PROX_TRIG);
    gpio_set_direction(IOTEM_GATE_PROX_TRIG, GPIO_MODE_OUTPUT);
    gpio_set_level(IOTEM_GATE_PROX_TRIG, 0);
}




void iotem_gate_open() {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL, IOTEM_GATE_PWM_OPEN);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL);
}




void iotem_gate_close() {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL, IOTEM_GATE_PWM_CLOSE);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, IOTEM_GATE_PWM_CHANNEL);
}




float iotem_gate_get_prox_dist() {
    uint32_t flight_time_us = 0;

    gpio_set_level(IOTEM_GATE_PROX_TRIG, 1);
    ets_delay_us(IOTEM_GATE_TRIG_ON_US);
    gpio_set_level(IOTEM_GATE_PROX_TRIG, 0);
    
    while(flight_time_us < IOTEM_GATE_ECHO_TIMEOUT_US) {
        if(gpio_get_level(IOTEM_GATE_PROX_ECHO) == 1) {
            return 1.0;
        }
        ets_delay_us(10);
        flight_time_us += 10;
    }

    return 0.0;
}