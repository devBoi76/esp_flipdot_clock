#pragma once
#include <cstdint>
#include <ctime>
#include <stddef.h>

enum GPZDA_Status {
  OK_WITH_CHECKSUM = 0,
  NOT_GPZDA = 1,
  ERR_TRAILING_COMMA = 2,
  ERR_FIRST_COMMA_MISSING = 3,
  ERR_BUFFER_OVERFLOW = 4,
  ERR_MALFORMED_TIME = 5,
  ERR_INVALID_MONTH = 6,
  ERR_INVALID_DAY = 7,
  ERR_INVALID_TZ = 8,
  ERR_INVALID_MZ = 9,
  ERR_MALFORMED_CHECKSUM = 10,
  OK_NO_CHECKSUM = 11,
  ERR_UNEXPECTED_COMMA = 12,
  ERR_UNEXPECTED_STAR = 13,
  ERR_INVALID_CHAR = 14,
  ERR_NO_GPS = 15,
};

typedef struct {
  GPZDA_Status status;
  int8_t hour = -1;
  uint8_t minute;
  uint8_t second;
  uint16_t millisecond;
  // 1-31
  uint8_t day;
  // 1-12
  uint8_t month;
  uint16_t year;
  int8_t timezone;
  int8_t timezone_min;
  uint8_t checksum;
} GPZDA_Data;

bool starts_with(const char* str, const char* prefix);
GPZDA_Data parse_gps_time_data(const char *line, size_t len);

struct GPSTimeUpdateData {
  struct tm utc_timeinfo;
  bool got_fix = false;
  bool is_gpzda = false;
  bool time_data_valid = true;
};

GPSTimeUpdateData parse_gps_message(const char *message, size_t len);
