#include "IPAddress.h"
#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time_data.hpp"

#define OLED_RESET -1
#define OLED_W 128
#define OLED_H 64

inline Adafruit_SSD1306 oled;

bool try_init_oled(TwoWire &wire);

void update_oled(bool got_time_at_all, bool gps_got_fix, TimeSource time_source, IPAddress ip_addr, float co2_ppm);