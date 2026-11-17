#pragma once

// Built-in 0.42" OLED of the ESP32-C3 mini board.
// The SSD1306 controller has a 128x64 frame buffer, but only a 72x40 window
// of it is wired to the glass.

#define OLED_SDA        5       // GPIO5
#define OLED_SCL        6       // GPIO6
#define OLED_RESET      -1      // no reset pin
#define OLED_ADDRESS    0x3C    // I2C address

#define OLED_CTRL_W     128     // controller frame buffer
#define OLED_CTRL_H     64

#define OLED_W          72      // visible window
#define OLED_H          40
#define OLED_X_OFFSET   28      // where the visible window starts in the buffer
#define OLED_Y_OFFSET   24
