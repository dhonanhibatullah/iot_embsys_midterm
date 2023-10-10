#include "include/iot_embsys_midterm_display.h"
#include "include/iot_embsys_midterm_keypad.h"
#include "include/iot_embsys_midterm_gate.h"




/* Global variable redeclaration */
SSD1306_t           display;

QueueHandle_t       password_queue;

SemaphoreHandle_t   auth_mutex;

uint8_t             auth_status = IOTEM_KEYPAD_AUTH_IDLE;

char                password[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1] = "12345678\0";




void iotem_display_task(void* pvParameters) {

    /* Local variables */
    uint8_t task_state = IOTEM_DISPLAY_SPLASH_SCREEN;
    bool    need_clear = true;
    char    password_buff[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1] = {0};

    /* Begin display */
    iotem_display_begin(&display);


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
                xSemaphoreTake(auth_mutex, portMAX_DELAY);
                switch(auth_status) {
                    
                    case IOTEM_KEYPAD_AUTH_IDLE:
                        break;

                    case IOTEM_KEYPAD_AUTH_SUCCESS:
                        task_state  = IOTEM_DISPLAY_ACCESS_GRANTED;
                        need_clear  = true;
                        auth_status = IOTEM_KEYPAD_AUTH_IDLE; 
                        break;

                    case IOTEM_KEYPAD_AUTH_FAILED:
                        task_state  = IOTEM_DISPLAY_ACCESS_DENIED;
                        need_clear  = true;
                        auth_status = IOTEM_KEYPAD_AUTH_IDLE;
                        break;
                }
                xSemaphoreGive(auth_mutex);

                ssd1306_display_text(&display, 0, "                ", 16, true);
                ssd1306_display_text(&display, 1, "Insert Password:", 16, true);
                ssd1306_display_text(&display, 2, "                ", 16, true);
                xQueueReceive(password_queue, &password_buff, portMAX_DELAY);
                ssd1306_display_text(&display, 5, password_buff, 16, false);
                break;


            case IOTEM_DISPLAY_ACCESS_GRANTED:
                ssd1306_display_text(&display, 2, "                ", 16, true);
                ssd1306_display_text(&display, 3, " ACCESS GRANTED!", 16, true);
                ssd1306_display_text(&display, 4, "                ", 16, true);
                break;


            case IOTEM_DISPLAY_ACCESS_DENIED:
                ssd1306_display_text(&display, 2, "                ", 16, true);
                ssd1306_display_text(&display, 3, " ACCESS DENIED! ", 16, true);
                ssd1306_display_text(&display, 4, "                ", 16, true);
                break;


            case IOTEM_DISPLAY_CHANGE_PASSWORD:
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
    uint8_t password_cursor     = 0;


    /* Begin keypad */
    iotem_keypad_begin();

    
    /* Begin password queue handler and auth semaphore handler */
    password_queue  = xQueueCreate(IOTEM_KEYPAD_PASSWORD_QUEUE_SIZE, sizeof(char[IOTEM_KEYPAD_PASSWORD_MAX_LEN + 1]));
    auth_mutex      = xSemaphoreCreateMutex();


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
                        if(password_cursor != 0) {
                            --password_cursor;
                            password_buff[password_cursor] = '\0';
                        }
                        break;
                    case IOTEM_KEYPAD_HASH:
                        if(password_cursor != 0) {
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
                            password_buff[0] = '\0';
                        }
                        break;
                    default:
                        break;
                }
            }


            else {
                switch(keypad_in) {
                    case IOTEM_KEYPAD_STAR:
                        --password_cursor;
                        password_buff[password_cursor] = '\0';
                        break;
                    case IOTEM_KEYPAD_HASH:
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
                        password_buff[0] = '\0';
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


    /* Begin gate components */
    iotem_gate_begin();


    /* Loop */
    while(true) {
        iotem_gate_open();
        vTaskDelay(pdMS_TO_TICKS(1500));
        iotem_gate_close();
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}



