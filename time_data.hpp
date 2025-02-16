#pragma once

#include "time.h"
#include "esp_sntp.h"


// current time
inline struct tm timeinfo = {.tm_sec = 50, .tm_min = 0, .tm_hour = 0};

enum TimeSource {
  GPS,
  NTP,
  LOCAL,
};

// Which time source is being used for getting the time
inline TimeSource time_source = LOCAL;
inline bool gps_got_fix = false;
inline bool got_time_at_all = false;
inline bool do_pick_time_source = true;


