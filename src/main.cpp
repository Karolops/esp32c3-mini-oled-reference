// Demo for the ESP32-C3 mini built-in 0.42" OLED (72x40).
// Cycles through 4 demo screens, 3 s each. Serial monitor: 115200.
#include <Arduino.h>
#include "mini_oled.h"

static MiniOled oled;

// 1. Frame + corner dots: checks the visible window / offset.
//    The whole frame and all four corner dots must be visible.
static void screenFrame() {
    oled.clear();
    oled.rect(0, 0, OLED_W, OLED_H);
    oled.rect(2, 2, 2, 2, true);
    oled.rect(OLED_W - 4, 2, 2, 2, true);
    oled.rect(2, OLED_H - 4, 2, 2, true);
    oled.rect(OLED_W - 4, OLED_H - 4, 2, 2, true);
    oled.textCentered(16, "72 x 40");
    oled.show();
}

// 2. Size 1 text: 5 rows x 12 chars fill the screen exactly.
static void screenTextSize1() {
    oled.clear();
    for (int row = 0; row < 5; row++) {
        char buf[13];
        snprintf(buf, sizeof(buf), "Row%d 1234567", row);
        oled.text(0, row * 8, buf);
    }
    oled.show();
}

// 3. Big value + small labels (typical live readout).
static void screenBigValue(float value) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%.0f", value);
    oled.clear();
    oled.text(0, 0, "TEST");
    oled.textRight(0, "unit");
    oled.textCentered(12, buf, 3);
    oled.show();
}

// 4. Label + percentage + bar + compact IP.
static void screenStatus(float fraction) {
    char pct[6];
    snprintf(pct, sizeof(pct), "%d%%", (int)(fraction * 100));
    oled.clear();
    oled.text(0, 0, "BAT");
    oled.textRight(0, pct);
    oled.bar(0, 10, OLED_W, 8, fraction);
    oled.textCompactIP(26, "192.168.1.23");
    oled.show();
}

void setup() {
    Serial.begin(115200);
    delay(200);
    if (!oled.begin()) {
        Serial.println("OLED not found - check SDA/SCL/address in oled_config.h");
        return;
    }
    Serial.println("OLED ok");
    oled.splash("Mini OLED", "demo");
    delay(1500);
}

void loop() {
    static uint8_t screen = 0;
    static uint32_t lastSwitch = 0;
    static uint32_t lastFrame = 0;
    uint32_t now = millis();

    if (now - lastSwitch > 3000) {
        lastSwitch = now;
        screen = (screen + 1) % 4;
    }
    if (now - lastFrame < 50) return;      // ~20 fps for the animated screens
    lastFrame = now;

    float t = (now % 4000) / 4000.0f;      // 0..1 sawtooth
    switch (screen) {
        case 0: screenFrame(); break;
        case 1: screenTextSize1(); break;
        case 2: screenBigValue(t * 1234.0f); break;
        case 3: screenStatus(t); break;
    }
}
