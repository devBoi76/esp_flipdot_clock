#pragma once

#include <Arduino.h>

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
