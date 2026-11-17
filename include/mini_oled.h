#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "oled_config.h"

// Thin wrapper around Adafruit_SSD1306 for the 72x40 visible window.
// All coordinates are LOCAL: (0,0) is the top-left visible pixel,
// (71,39) the bottom-right. The offset into the 128x64 buffer is hidden here.
//
// Font sizes (built-in 5x7 font + 1 px spacing):
//   size 1:  6x8  px per char -> 12 chars x 5 rows
//   size 2: 12x16 px per char ->  6 chars x 2 rows
//   size 3: 18x24 px per char ->  4 chars x 1 row
class MiniOled {
public:
    bool begin();                          // false = display not found on I2C

    void clear();                          // clear buffer (call show() to apply)
    void show();                           // push buffer to the display

    // Text
    void text(int x, int y, const char* s, uint8_t size = 1);
    void textCentered(int y, const char* s, uint8_t size = 1);
    void textRight(int y, const char* s, uint8_t size = 1);
    void textCompactIP(int y, const char* ip);   // dots 3 px wide, centred

    // Graphics
    void pixel(int x, int y, bool on = true);
    void line(int x0, int y0, int x1, int y1);
    void rect(int x, int y, int w, int h, bool filled = false);
    void bar(int x, int y, int w, int h, float fraction);   // outlined bar, 0..1
    void invert(bool on);                  // hardware invert of the whole screen
    void contrast(uint8_t level);          // 0..255

    // One or two centred lines (boot / error screens); draws and shows
    void splash(const char* line1, const char* line2 = nullptr);

    static int textWidth(const char* s, uint8_t size = 1) { return strlen(s) * 6 * size; }

    // Full Adafruit API. Careful: it uses BUFFER coordinates (add the offsets).
    Adafruit_SSD1306& raw() { return _d; }

private:
    Adafruit_SSD1306 _d{OLED_CTRL_W, OLED_CTRL_H, &Wire, OLED_RESET};
    static int X(int x) { return x + OLED_X_OFFSET; }
    static int Y(int y) { return y + OLED_Y_OFFSET; }
};
