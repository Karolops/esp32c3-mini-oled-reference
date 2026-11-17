#include "mini_oled.h"
#include <Wire.h>

bool MiniOled::begin() {
    Wire.begin(OLED_SDA, OLED_SCL);
    if (!_d.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) return false;
    _d.setTextColor(SSD1306_WHITE);
    _d.setTextWrap(false);                 // never wrap into the invisible buffer area
    clear();
    show();
    return true;
}

void MiniOled::clear() { _d.clearDisplay(); }
void MiniOled::show()  { _d.display(); }

void MiniOled::text(int x, int y, const char* s, uint8_t size) {
    _d.setTextSize(size);
    _d.setCursor(X(x), Y(y));
    _d.print(s);
}

void MiniOled::textCentered(int y, const char* s, uint8_t size) {
    text(max(0, (OLED_W - textWidth(s, size)) / 2), y, s, size);
}

void MiniOled::textRight(int y, const char* s, uint8_t size) {
    text(max(0, OLED_W - textWidth(s, size)), y, s, size);
}

// At size 1 an IP like 192.168.1.23 is 12 chars = 72 px, the whole width.
// Drawing each dot as one pixel in a 3 px cell saves 3 px per dot.
// Longest IPs (xxx.xxx.xxx.xxx = 81 px) still do not fit completely.
void MiniOled::textCompactIP(int y, const char* ip) {
    int total = 0;
    for (const char* p = ip; *p; p++) total += (*p == '.') ? 3 : 6;
    int cx = max(0, (OLED_W - total) / 2);
    _d.setTextSize(1);
    for (const char* p = ip; *p; p++) {
        if (*p == '.') {
            pixel(cx + 1, y + 6);
            cx += 3;
        } else {
            _d.setCursor(X(cx), Y(y));
            _d.print(*p);
            cx += 6;
        }
    }
}

void MiniOled::pixel(int x, int y, bool on) {
    _d.drawPixel(X(x), Y(y), on ? SSD1306_WHITE : SSD1306_BLACK);
}

void MiniOled::line(int x0, int y0, int x1, int y1) {
    _d.drawLine(X(x0), Y(y0), X(x1), Y(y1), SSD1306_WHITE);
}

void MiniOled::rect(int x, int y, int w, int h, bool filled) {
    if (filled) _d.fillRect(X(x), Y(y), w, h, SSD1306_WHITE);
    else        _d.drawRect(X(x), Y(y), w, h, SSD1306_WHITE);
}

void MiniOled::bar(int x, int y, int w, int h, float fraction) {
    if (fraction < 0) fraction = 0;
    if (fraction > 1) fraction = 1;
    rect(x, y, w, h);
    int inner = (int)((w - 4) * fraction + 0.5f);
    if (inner > 0 && h > 4) rect(x + 2, y + 2, inner, h - 4, true);
}

void MiniOled::invert(bool on) { _d.invertDisplay(on); }

void MiniOled::contrast(uint8_t level) {
    _d.ssd1306_command(SSD1306_SETCONTRAST);
    _d.ssd1306_command(level);
}

void MiniOled::splash(const char* line1, const char* line2) {
    clear();
    if (line2) {
        textCentered(8, line1);
        textCentered(24, line2);
    } else {
        textCentered(16, line1);
    }
    show();
}
