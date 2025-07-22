
#include "gps_interface.hpp"
#include <cstdlib>
#include <cstring>
#include <Arduino.h>
#include "util.h"


bool starts_with(const char* str, const char* prefix) {
    while (*prefix && *str && *str == *prefix) ++str, ++prefix;
    return *prefix == 0;
}

typedef struct {
  GPZDA_Status status;
  int number;
  size_t advance;
} DigitReturn;

// parse n digits into an integer. status is 0 (success), ERR_BUFFER_OVERFLOW, or ERR_INVALID_CHAR
DigitReturn parse_integer(const char *str, size_t num_digits) {
  DigitReturn ret = {.status = OK_WITH_CHECKSUM, .number = 0, .advance = num_digits};

  if (strlen(str) < num_digits) {
    ret.status = ERR_BUFFER_OVERFLOW;
    return ret;
  }

  bool is_negative = str[0] == '-';
  int start = is_negative ? 1 : 0;
  if (is_negative) ret.advance += 1;

  for (int i = start; i < num_digits + start; i += 1) {
    if ('0' <= str[i] && str[i] <= '9') {
      ret.number *= 10;
      ret.number += str[i] - '0';
    } else {
      ret.status = ERR_INVALID_CHAR;
      return ret;
    }
  }

  return ret;
}

enum GPZDA_Field {
    none = 0,
    // time
    hhmmss_sss = 1,
    // day 1..31
    dd = 2,
    // month 1..12
    mm = 3,
    // year
    yyyy = 4,
    // Local zone description, 00 to +/- 13 hours
    tz = 5,
    // Local zone minutes description (same sign as hours)
    mz = 6,
    checksum = 7,
};


GPZDA_Data parse_gps_time_data(const char *line, size_t len) {
  GPZDA_Data ret = {.status = NOT_GPZDA};
  GPZDA_Field field = none;
  DigitReturn parse_ret;
  if (!starts_with(line, "$GPZDA")) return ret;

  size_t idx = 6;

  while (true) {
    // field separator
    if (line[idx] == ',') {
      switch (field) {
        case none: field = hhmmss_sss; break;
        case hhmmss_sss: field = dd; break;
        case dd: field = mm; break;
        case mm: field = yyyy; break;
        case yyyy: field = tz; break;
        case tz: field = mz; break;
        case mz:
          ret.status = ERR_UNEXPECTED_COMMA;
          return ret;
        case checksum:
          // error: there should not be a , after the checksum
          ret.status = ERR_TRAILING_COMMA;
          return ret;
      }

      // advance after comma
      if (idx + 1 < len) {
        idx += 1;
      } else {
        ret.status = ERR_BUFFER_OVERFLOW;
        return ret;
      }

    } else if (line[idx] == '*') {
      // checsum separator
      if (field == mz) {
        field = checksum;
      } else {
        ret.status = ERR_UNEXPECTED_STAR;
        return ret;
      }

      // advance after *
      if (idx + 1 < len) {
        idx += 1;
      } else {
        ret.status = ERR_BUFFER_OVERFLOW;
        return ret;
      }

    } else {
      // parse fields
      switch (field) {
        case none: {
          ret.status = ERR_FIRST_COMMA_MISSING;
          return ret;
        }
        case hhmmss_sss: {
          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.hour = parse_ret.number;
          idx += parse_ret.advance;

          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.minute = parse_ret.number;
          idx += parse_ret.advance;


          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.second = parse_ret.number;
          idx += parse_ret.advance;

          if (line[idx] != '.') {
            ret.status = ERR_MALFORMED_TIME;
            return ret;
          } else {
            idx += 1;
          }


          parse_ret = parse_integer(line+idx, 3);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.millisecond = parse_ret.number;
          idx += parse_ret.advance;

          break;
        }
        case mm: {
          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.month = parse_ret.number;
          idx += parse_ret.advance;

          if (ret.month > 12) {
            ret.status = ERR_INVALID_MONTH;
            return ret;
          }

          break;
        }
        // day
        case dd: {
          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.day = parse_ret.number;
          idx += parse_ret.advance;

          if (ret.day > 31) {
            ret.status = ERR_INVALID_DAY;
            return ret;
          }

          break;
        }
        // year
        case yyyy: {
          parse_ret = parse_integer(line+idx, 4);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.year = parse_ret.number;
          idx += parse_ret.advance;

          break;
        }
        // time zone hour offset
        case tz: {
          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.timezone = parse_ret.number;
          idx += parse_ret.advance;

          if (ret.timezone > 13 || ret.timezone < -13) {
            ret.status = ERR_INVALID_TZ;
            return ret;
          }

          break;
        }
        // time zone minute offset
        case mz: {
          parse_ret = parse_integer(line+idx, 2);
          if (parse_ret.status != 0) {ret.status = parse_ret.status; return ret;}
          ret.timezone_min = parse_ret.number;
          idx += parse_ret.advance;

          if (ret.timezone_min > 60 || ret.timezone_min < -60) {
            ret.status = ERR_INVALID_MZ;
            return ret;
          }

          break;
        }
        // "*XX" where XX is in hex
        case checksum: {
          const size_t field_len = 2;
          if (ret.hour == -1) {
            ret.status = ERR_NO_GPS;
            return ret;
          }
          // no checksum
          if (idx + field_len >= len) {
            ret.status = OK_NO_CHECKSUM;
            return ret;
          }

          ret.checksum = 0;
          if (line[idx] >= '0' && line[idx] <= '9') {
            ret.checksum += (line[idx] - '0') * 16;
          } else if (line[idx] >= 'A' && line[idx] <= 'F') {
            ret.checksum += (line[idx] - 'A' + 10) * 16;
          } else {
            ret.status = ERR_MALFORMED_CHECKSUM;
            return ret;
          }

          if (line[idx+1] >= '0' && line[idx+1] <= '9') {
            ret.checksum += (line[idx+1] - '0');
          } else if (line[idx+1] >= 'A' && line[idx+1] <= 'F') {
            ret.checksum += (line[idx+1] - 'A' + 10);
          } else {
            ret.status = ERR_MALFORMED_CHECKSUM;
            return ret;
          }

          ret.status = OK_WITH_CHECKSUM;
          return ret;
        }
      }
    }
  }
}

GPSTimeUpdateData parse_gps_message(const char *message, size_t len) {


  GPSTimeUpdateData ret;

  GPZDA_Data time_data = parse_gps_time_data(message, len);

  ret.is_gpzda = (time_data.status != NOT_GPZDA);
  ret.got_fix = (time_data.status != ERR_NO_GPS);

  switch(time_data.status) {
    case NOT_GPZDA: return ret;
    case ERR_NO_GPS: return ret;
    case OK_WITH_CHECKSUM: break;
    case OK_NO_CHECKSUM: break;
    // time data, which we care about might be invalid
    case ERR_MALFORMED_TIME:
    case ERR_INVALID_CHAR:
    case ERR_BUFFER_OVERFLOW:
      slog("[critical] GPZDA parse error: ");
      slogln(time_data.status);
      ret.time_data_valid = false;
      break;
    // other non-critical errors
    default:
      slog("GPZDA parse error: ");
      slogln(time_data.status);
      break;
  }

  ret.utc_timeinfo.tm_hour = time_data.hour;
  ret.utc_timeinfo.tm_min = time_data.minute;
  ret.utc_timeinfo.tm_sec = time_data.second;

  if (time_data.status == ERR_INVALID_DAY) return ret;
  ret.utc_timeinfo.tm_mday = time_data.day;

  if (time_data.status == ERR_INVALID_MONTH) return ret;
  ret.utc_timeinfo.tm_mon = time_data.month - 1;
  // struct tm expects years from 1900
  ret.utc_timeinfo.tm_year = time_data.year - 1900;

  return ret;

}
