#include "flipdot_interface.hpp"
#include "clock_display.hpp"
#include <cstdint>
#include <Arduino.h>

#define ERR_X_TOO_BIG -1;
#define ERR_Y_TOO_BIG -2;

int set_pixel(uint8_t x_coord, uint8_t y_coord, bool state) {
    if (x_coord > SCREEN_W) return ERR_X_TOO_BIG;
    if (y_coord > SCREEN_H) return ERR_Y_TOO_BIG;

    uint8_t x_bytes = x_coord + 1;
    uint8_t y_bytes = (y_coord+1) * 2 + (state ? 1 : 0);
    uint8_t x_message = ((x_bytes & 0b11111) << 3) | state;
    uint8_t y_message = ((y_bytes & 0b11111) << 3) | 1;

    for (int i = 0; i < 8; i++) {
        // Send the messages to the flipdot display
        uint8_t x_bit = (x_message >> i) & 1;
        uint8_t y_bit = (y_message >> i) & 1;
        digitalWrite(x_bit, X_DATA_PIN);
        digitalWrite(y_bit, Y_DATA_PIN);
        digitalWrite(1, CLOCK_PIN);
        digitalWrite(0, CLOCK_PIN);
    }

    return 0;
}
