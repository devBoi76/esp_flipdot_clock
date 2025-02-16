#pragma once

#include <Arduino.h>
#include <time.h>
#include <stdlib.h>

extern bool show_log();

template <typename T>
void slog(T v) {
  if (show_log()) {
    Serial.print(v);
  }
}

template <typename T>
void slogln(T v) {
  if (show_log()) {
    Serial.println(v);
  }
}

inline time_t
my_timegm(struct tm *tm)
{
    time_t ret;
    char *tz;

    tz = getenv("TZ");
    if (tz)
        tz = strdup(tz);
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm);
    if (tz) {
        setenv("TZ", tz, 1);
        free(tz);
    } else
        unsetenv("TZ");
    tzset();
    return ret;
}
