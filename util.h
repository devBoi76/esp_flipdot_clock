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

// // copies string
// int copy_string (char *to, const char* from, size_t max_from_size) {

// }