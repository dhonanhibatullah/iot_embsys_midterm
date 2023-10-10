#include "include/iot_embsys_midterm_display.h"
#include "include/iot_embsys_midterm_keypad.h"
#include "include/iot_embsys_midterm_gate.h"
#include "include/iot_embsys_midterm_uart.h"




/* Global variable redeclaration */
SSD1306_t           display;

QueueHandle_t       password_queue;

SemaphoreHandle_t   auth_mutex,
                    gate_mutex,
                    password_mutex,
                    change_password_mutex;

uint8_t             auth_status = IOTEM_KEYPAD_AUTH_IDLE,
                    gate_status = IOTEM_GATE_CLOSED,
                    change_password_req = IOTEM_DISPLAY_CHANGE_PASSWORD_YET;

char                password[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1] = "12345678\0";




void iotem_display_task(void* pvParameters) {

    /* Local variables */
    uint8_t task_state = IOTEM_DISPLAY_SPLASH_SCREEN;
    bool    need_clear = true;
    char    password_buff[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1] = {0};


    /* Begin display */
    iotem_display_begin(&display);


    /* Create change password mutex */
    change_password_mutex = xSemaphoreCreateMutex();


    /* Loop */
    while(true) {

        /* Clear screen if needed */        
        if(need_clear) {
            ssd1306_clear_screen(&display, false);
            need_clear = false;
        }
        

        /* Switch cases */
        switch(task_state) {

            
            case IOTEM_DISPLAY_SPLASH_SCREEN:
                ssd1306_display_text(&display, 0, "                ", 16, true);
                ssd1306_display_text(&display, 1, " [ SMART GATE ] ", 16, true);
                ssd1306_display_text(&display, 2, "                ", 16, true);
                ssd1306_display_text(&display, 4, "  Dhonan  N.H.  ", 16, false);
                ssd1306_display_text(&display, 6, "  IoT Midterm   ", 16, false);
                vTaskDelay(pdMS_TO_TICKS(IOTEM_DISPLAY_SPLASH_SCREEN_MS));
                task_state = IOTEM_DISPLAY_PASSWORD;
                need_clear = true;
                break;


            case IOTEM_DISPLAY_PASSWORD:
                /* Check for password change request */
                xSemaphoreTake(change_password_mutex, portMAX_DELAY);
                if(change_password_req == IOTEM_DISPLAY_CHANGE_PASSWORD_REQ) {
                    task_state = IOTEM_DISPLAY_CHANGE_PASSWORD;
                    need_clear = true;
                }
                xSemaphoreGive(change_password_mutex);

                /* Check for authentification */
                xSemaphoreTake(auth_mutex, portMAX_DELAY);
                switch(auth_status) {
                    
                    case IOTEM_KEYPAD_AUTH_IDLE:
                        break;

                    case IOTEM_KEYPAD_AUTH_SUCCESS:
                        auth_status = IOTEM_KEYPAD_AUTH_IDLE;
                        task_state  = IOTEM_DISPLAY_ACCESS_GRANTED;
                        need_clear  = true;
                        xSemaphoreTake(gate_mutex, portMAX_DELAY);
                        gate_status = IOTEM_GATE_OPENED;
                        xSemaphoreGive(gate_mutex);
                        break;

                    case IOTEM_KEYPAD_AUTH_FAILED:
                        auth_status = IOTEM_KEYPAD_AUTH_IDLE;
                        task_state  = IOTEM_DISPLAY_ACCESS_DENIED;
                        need_clear  = true;
                        break;
                }
                xSemaphoreGive(auth_mutex);
                if(task_state != IOTEM_DISPLAY_PASSWORD) break;

                ssd1306_display_text(&display, 0, "                ", 16, true);
                ssd1306_display_text(&display, 1, "Insert Password:", 16, true);
                ssd1306_display_text(&display, 2, "                ", 16, true);

                /* Receive user input queue from keypad */
                xQueueReceive(password_queue, &password_buff, pdMS_TO_TICKS(IOTEM_DISPLAY_PASSWORD_QUEUE_TIMEOUT_MS));
                ssd1306_display_text(&display, 5, password_buff, 16, false);
                break;


            case IOTEM_DISPLAY_ACCESS_GRANTED:
                /* Display when access is granted */
                ssd1306_display_text(&display, 2, "                ", 16, true);
                ssd1306_display_text(&display, 3, " ACCESS GRANTED!", 16, true);
                ssd1306_display_text(&display, 4, "                ", 16, true);

                xSemaphoreTake(gate_mutex, portMAX_DELAY);
                if(gate_status == IOTEM_GATE_CLOSED) {
                    task_state = IOTEM_DISPLAY_PASSWORD;
                    need_clear = true;
                }
                xSemaphoreGive(gate_mutex);
                break;


            case IOTEM_DISPLAY_ACCESS_DENIED:
                /* Display when access is denied */
                ssd1306_display_text(&display, 2, "                ", 16, true);
                ssd1306_display_text(&display, 3, " ACCESS DENIED! ", 16, true);
                ssd1306_display_text(&display, 4, "                ", 16, true);
                vTaskDelay(pdMS_TO_TICKS(IOTEM_DISPLAY_ACCESS_DENIED_MS));
                task_state = IOTEM_DISPLAY_PASSWORD;
                need_clear = true;
                break;


            case IOTEM_DISPLAY_CHANGE_PASSWORD:
                /* Display when password change */
                ssd1306_display_text(&display, 2, "                ", 16, true);
                ssd1306_display_text(&display, 3, " PASSWORD CHANGE", 16, true);
                ssd1306_display_text(&display, 4, "                ", 16, true);
                xSemaphoreTake(change_password_mutex, portMAX_DELAY);
                if(change_password_req == IOTEM_DISPLAY_CHANGE_PASSWORD_YET) {
                    task_state = IOTEM_DISPLAY_PASSWORD;
                    need_clear = true;
                }
                xSemaphoreGive(change_password_mutex);
                break;


            default:
                break;
        }


        /* Task delay */
        vTaskDelay(pdMS_TO_TICKS(IOTEM_DISPLAY_TASK_DELAY_MS));
    }
}




void iotem_keypad_task(void* pvParameters) {

    /* Local variables */
    char    password_buff[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1]   = {0};
    uint8_t password_cursor = 0;


    /* Begin keypad */
    iotem_keypad_begin();

    
    /* Begin password queue handler and semaphore handlers */
    password_queue  = xQueueCreate(IOTEM_KEYPAD_PASSWORD_QUEUE_SIZE, sizeof(char[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1]));
    auth_mutex      = xSemaphoreCreateMutex();
    password_mutex  = xSemaphoreCreateMutex();


    /* Loop */
    while(true) {
        
        /* Read keypad input */
        uint8_t keypad_in = iotem_keypad_read();

        if(keypad_in != IOTEM_KEYPAD_NULL) {
        
            if(password_cursor < IOTEM_KEYPAD_PASSWORD_MAX_LEN) {
                switch(keypad_in) {
                    case IOTEM_KEYPAD_1:        
                        password_buff[password_cursor] = '1';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_2:        
                        password_buff[password_cursor] = '2';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_3:        
                        password_buff[password_cursor] = '3';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_4:        
                        password_buff[password_cursor] = '4';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_5:        
                        password_buff[password_cursor] = '5';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_6:        
                        password_buff[password_cursor] = '6';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_7:        
                        password_buff[password_cursor] = '7';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_8:        
                        password_buff[password_cursor] = '8';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_9:        
                        password_buff[password_cursor] = '9';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_0:        
                        password_buff[password_cursor] = '0';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_A:        
                        password_buff[password_cursor] = 'A';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_B:        
                        password_buff[password_cursor] = 'B';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_C:        
                        password_buff[password_cursor] = 'C';
                        ++password_cursor;
                        break;
                    case IOTEM_KEYPAD_D:
                        password_buff[password_cursor] = 'D';
                        ++password_cursor;
                        break;
                    
                    case IOTEM_KEYPAD_STAR:
                        /* Delete user input */
                        if(password_cursor != 0) {
                            --password_cursor;
                            password_buff[password_cursor] = '\0';
                        }
                        break;
                        
                    case IOTEM_KEYPAD_HASH:
                        if(password_cursor != 0) {
                            /* Authentication notification */
                            xSemaphoreTake(password_mutex, portMAX_DELAY);
                            if(iotem_keypad_auth(password_buff, password)) {
                                xSemaphoreTake(auth_mutex, portMAX_DELAY);
                                auth_status = IOTEM_KEYPAD_AUTH_SUCCESS;
                                xSemaphoreGive(auth_mutex);
                            }
                            else {
                                xSemaphoreTake(auth_mutex, portMAX_DELAY);
                                auth_status = IOTEM_KEYPAD_AUTH_FAILED;
                                xSemaphoreGive(auth_mutex);
                            }
                            xSemaphoreGive(password_mutex);

                            /* Password buffer reset */
                            for(uint8_t i = 0; i < IOTEM_KEYPAD_PASSWORD_MAX_LEN; ++i) {
                                password_buff[i] = '\0';
                            }
                            password_cursor = 0;
                        }
                        break;
                    default:
                        break;
                }
            }


            else {
                switch(keypad_in) {
                    
                    case IOTEM_KEYPAD_STAR:
                        /* Delete user input */
                        --password_cursor;
                        password_buff[password_cursor] = '\0';
                        break;

                    case IOTEM_KEYPAD_HASH:
                        /* Authentication notification */
                        xSemaphoreTake(password_mutex, portMAX_DELAY);
                        if(iotem_keypad_auth(password_buff, password)) {
                            xSemaphoreTake(auth_mutex, portMAX_DELAY);
                            auth_status = IOTEM_KEYPAD_AUTH_SUCCESS;
                            xSemaphoreGive(auth_mutex);
                        }
                        else {
                            xSemaphoreTake(auth_mutex, portMAX_DELAY);
                            auth_status = IOTEM_KEYPAD_AUTH_FAILED;
                            xSemaphoreGive(auth_mutex);
                        }
                        xSemaphoreGive(password_mutex);

                        /* Password buffer reset */
                        for(uint8_t i = 0; i < IOTEM_KEYPAD_PASSWORD_MAX_LEN; ++i) {
                            password_buff[i] = '\0';
                        }
                        password_cursor = 0;
                        break;
                    default:
                        break;
                }
            }


            /* Send the data to queue */
            xQueueSend(password_queue, &password_buff, portMAX_DELAY);
        }


        /* Delay */
        vTaskDelay(pdMS_TO_TICKS(IOTEM_KEYPAD_TASK_DELAY_MS));
    }
}




void iotem_gate_task(void* pvParameters) {

    /* Local variables */
    uint8_t gate_cmd    = IOTEM_GATE_CLOSED;

    
    /* Begin gate components */
    iotem_gate_begin();


    /* Begin gate mutex */
    gate_mutex = xSemaphoreCreateMutex();


    /* Loop */
    while(true) {

        /* Receive gate status via mutex */
        xSemaphoreTake(gate_mutex, portMAX_DELAY);
        gate_cmd = gate_status;
        xSemaphoreGive(gate_mutex);


        /* Open if gate open command received */
        if(gate_cmd == IOTEM_GATE_OPENED) {

            /* Open gate */
            iotem_gate_open();

            
            /* Check whether the user has passed the gate or not via distance sensor */
            while(iotem_gate_get_pass() == IOTEM_GATE_NOT_PASSED) {
                vTaskDelay(pdMS_TO_TICKS(IOTEM_GATE_PASS_DELAY_MS));
            }


            /* Close gate */
            iotem_gate_close();


            /* Reset */
            xSemaphoreTake(gate_mutex, portMAX_DELAY);
            gate_status = IOTEM_GATE_CLOSED;
            xSemaphoreGive(gate_mutex);
        }


        /* Delay */
        vTaskDelay(pdMS_TO_TICKS(IOTEM_GATE_TASK_DELAY_MS));
    }
}




void iotem_uart_task(void* pvParameters) {

    /* Local variables */
    char buf[128];
    uint8_t task_state = IOTEM_PASSWORD_IDLE;


    /* Begin UART communication */
    iotem_uart_begin();


    /* Loop */
    while(true) {

        /* Read UART */
        int len = uart_read_bytes(UART_NUM_0, buf, sizeof(buf), pdMS_TO_TICKS(IOTEM_UART_DELAY_MS));


        /* If UART buffer detected */
        if(len > 0) {
            /* Null terminate the last and before the last entry (clear whitespaces) */
            buf[len]        = '\0';
            buf[len - 1]    = '\0';
            
            
            switch(task_state) {
                
                case IOTEM_PASSWORD_IDLE: 
                    /* Change password can be done iff the input is the same as current password */
                    if(strcmp(buf, password) == 0) {
                        xSemaphoreTake(change_password_mutex, portMAX_DELAY);
                        change_password_req = IOTEM_DISPLAY_CHANGE_PASSWORD_REQ;
                        xSemaphoreGive(change_password_mutex);

                        printf("Insert new password (1-9 and capital A-D only):\n");
                        task_state = IOTEM_PASSWORD_CHANGE;
                    }
                    break;

                case IOTEM_PASSWORD_CHANGE:
                    /* Change password */
                    xSemaphoreTake(password_mutex, portMAX_DELAY);
                    for(uint8_t i = 0; i < IOTEM_KEYPAD_PASSWORD_MAX_LEN; ++i) {
                        password[i] = buf[i];
                    }
                    xSemaphoreGive(password_mutex);
                    
                    /* Done change password request */
                    xSemaphoreTake(change_password_mutex, portMAX_DELAY);
                    change_password_req = IOTEM_DISPLAY_CHANGE_PASSWORD_YET;
                    xSemaphoreGive(change_password_mutex);
                    printf("Password changed successfully!\n");

                    task_state = IOTEM_PASSWORD_IDLE;
            }
        }


        /* Delay */
        vTaskDelay(pdMS_TO_TICKS(IOTEM_UART_TASK_DELAY_MS));
    }
}