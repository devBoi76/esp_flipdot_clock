#pragma once
#include "clock_display.hpp"

void flipdot_set_pixel(uint8_t addr_x, uint8_t addr_y, bool state);
void flipdot_init();

void flipdot_write_screen(screen_cursor_t *cur);