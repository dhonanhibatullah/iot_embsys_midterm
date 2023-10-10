#include "include/iot_embsys_midterm_display.h"




void iotem_display_begin(SSD1306_t* _display) {

    /* Start I2C master for SSD1306 */
    i2c_master_init(_display, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    

    /* Initiate SSD1306 */
    ssd1306_init(_display, IOTEM_SSD1306_WIDTH, IOTEM_SSD1306_HEIGHT);
    ssd1306_clear_screen(_display, false);
	ssd1306_contrast(_display, IOTEM_SSD1306_CONTRAST);
}