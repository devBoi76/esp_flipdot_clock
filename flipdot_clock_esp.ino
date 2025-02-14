#include <Preferences.h>


#include <Wire.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <WiFi.h>
#include <Arduino.h>


#define SCREEN_H 9
#define SCREEN_W 28

#include "util.h"

#include "time_data.hpp"
#include "clock_display.hpp"
#include "gps_interface.hpp"
#include "scd30_interface.hpp"
#include "oled_display.hpp"

// wifi config
#define SSID_PREF "ssid"
#define PASS_PREF "password"

bool wifi_set_correctly = false;
char wifi_ssid[33];//"moto g84 5G Jan";
char wifi_password[128] = "niepowiem";

Preferences preferences;

const char *ntpServer = "pool.ntp.org";
const long  gmt_offset_sec = 3600;
const int   daylight_offset_sec = 3600;

// uart config
const int RXD1 = 22;
const int TXD1 = 23;

const int RXD2 = 13;
const int TXD2 = 15;
const int GPS_BAUD_RATE = 9600;
#define GPS_UART Serial2



// photodiode config
const int PHOTODIODE = 39;
const int DISPLAY_TRESHOLD = 300;



// current co2, temp, and humidity
float co2 = -100;
float temperature = -100;
float humidity = -100;
bool scd30_data_valid = false;

void update_time_gps();

bool try_connect_wifi(const char *ssid, const char *password) {
  if (!wifi_set_correctly) {
    return false;
  }

  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);

  if (WiFi.isConnected()) {
    WiFi.disconnect(true);
  }
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (true) {

    switch(WiFi.status()) {
      case WL_CONNECTED:
        Serial.println("Connected");
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());
        configTime(gmt_offset_sec, daylight_offset_sec, ntpServer);
        time_source = NTP;
        return true;
      case WL_NO_SSID_AVAIL:
        Serial.print("Error: wifi SSID not found:\t`");
        Serial.print(ssid);
        Serial.println('`');
        time_source = GPS;
        return false;
      case WL_CONNECT_FAILED:
      case WL_CONNECTION_LOST:
        Serial.println("WiFi connection failed. retrying");
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        break;
      default:
        Serial.print(".");
        delay(100);
        break;
    }
  }
}

enum ConfigState : unsigned char {
  NONE,
  SSID,
  PASSWD,
};
ConfigState config_state = NONE;
bool show_log() {
  return config_state == NONE;
}
char pc_rx_buf[512];
char new_ssid[33];
char new_password[128];
void on_recieve_computer_data() {
  const int n = Serial.readBytesUntil('\n', pc_rx_buf, sizeof(pc_rx_buf));
  if (n == 0) return;

  if (config_state == NONE) {
    config_state = SSID;
    if (!starts_with(pc_rx_buf, "config")) {
      Serial.println("Unknown command. Type `config` to configure wifi.");
      return;
    }
    Serial.print("Enter wifi SSID (name): ");
  } else if (config_state == SSID) {
    config_state = PASSWD;
    strncpy(new_ssid, pc_rx_buf, n);
    // new_ssid[n] = 0;
    Serial.println("Enter wifi password: ");
  } else if (config_state == PASSWD) {
    config_state = NONE;
    strncpy(new_password, pc_rx_buf, n);
    // new_password[n] = 0;
    wifi_set_correctly = true;
    // if (try_connect_wifi(new_ssid, new_password)) {
    strncpy(wifi_ssid, new_ssid, strlen(new_ssid));
    strncpy(wifi_password, new_password, strlen(new_password));

    // save new credentials
    preferences.begin("esp-clock", false);

    preferences.putString(SSID_PREF, wifi_ssid);
    preferences.putString(PASS_PREF, wifi_password);

    preferences.end();

    do_pick_time_source = true;
    // } else wifi_set_correctly = false;
  }


}

void setup() {
  Serial.begin(115200);
  // init screen
  screen_chars[n_screen_chars] = 0;
  init_cursor(&screen_cursor);

  Serial.println("ESP32 Flipdot display clock");

  // init preferences
  preferences.begin("esp-clock", false);

  preferences.getString(SSID_PREF, wifi_ssid, sizeof wifi_ssid);
  preferences.getString(PASS_PREF, wifi_password, sizeof wifi_password);

  preferences.end();

  if (strlen(wifi_ssid) != 0 && strlen(wifi_password) != 0)
    wifi_set_correctly = true;

  // init wifi
  try_connect_wifi(wifi_ssid, wifi_password);

  // init gps
  GPS_UART.begin(GPS_BAUD_RATE, SERIAL_8N1, RXD2, TXD2);
  GPS_UART.onReceive(update_time_gps);
  Serial.onReceive(on_recieve_computer_data);

  Serial.println("[Serial] init success.");

  // init oled
  if (!try_init_oled(Wire)) {
    Serial.println("[OLED] init failed.");
  } else {
    Serial.println("[OLED] init success");
  }

  // init scd30
  scd30_init_success = try_init_scd30_sensor(Wire1);
  if (scd30_init_success == false) {
    Serial.println("[SCD30] init failed.");
  } else {
    Serial.println("[SCD30] init success.");
  }
  
}

void resolve_time_data() {
  do_pick_time_source |= !WiFi.isConnected();
  if (do_pick_time_source) {
    if(try_connect_wifi(wifi_ssid, wifi_password)) {
      time_source = NTP;
      do_pick_time_source = false;
    } else if (gps_got_fix) {
      time_source = GPS;
    } else {
      time_source = LOCAL;
    }
  }

  if (time_source != GPS) {
    if (getLocalTime(&timeinfo)) {
      got_time_at_all = true;
    } else {
      slogln("Failed to obtain ntp time.");
      do_pick_time_source = true;
    }
  }
}

void update_screen() {
  if (!wifi_set_correctly) { slogln("WiFi not configured. Type `configure`."); }

  slog("Current time source: ");
  if (time_source == GPS) slog("GPS\n");
  else if (time_source == NTP) slog("NTP\n");
  else if (time_source == LOCAL) slog("Device local time\n");

  slog("Got GPS fix?: ");
  slogln(gps_got_fix == true);

  int light_level = analogRead(PHOTODIODE);
  if (light_level < DISPLAY_TRESHOLD) {
    slog("Light too low to update screen.");
    return;
  }

  if (got_time_at_all == false) {
    screen_chars[0] = 'A';
    screen_chars[1] = 'G';
    screen_chars[2] = 'H';
  } else {
    screen_chars[0] = '0' + (timeinfo.tm_hour / 10);
    screen_chars[1] = '0' + (timeinfo.tm_hour % 10);
    screen_chars[2] = ':';
    screen_chars[3] = '0' + (timeinfo.tm_min / 10);
    screen_chars[4] = '0' + (timeinfo.tm_min % 10);
  }

  clear_screen(&screen_cursor);

  int ret = put_string(&screen_cursor, screen_chars);
  if (ret != 0) {
    Serial.print("SCREEN ERROR: ");
    Serial.println(ret);
  }
  set_mask(&screen_cursor, 1);
  print_screen(&screen_cursor);
}



void loop() {
  int start = millis();

  while (!got_time_at_all) {
    int ret;
    ret = put_string_animation(&screen_cursor, " AGH", 200);
    if (ret != 0) {
      Serial.print("[SCREEN ERROR]: ");
      Serial.println(ret);
    }
    
    resolve_time_data();

    ret = put_string_animation(&screen_cursor, " AGH", 200, true);
    if (ret != 0) {
      Serial.print("[SCREEN ERROR]: ");
      Serial.println(ret);
    }
    resolve_time_data();
  }

  resolve_time_data();
  scd30_data_valid = poll_scd30_data(co2, temperature, humidity);
  if (scd30_data_valid) {
    slogln(co2);
    slogln(temperature);
    slogln(humidity);
  } else {
    slogln("SCD30 data not ready");
  }

  update_screen();
  update_oled(got_time_at_all, gps_got_fix, time_source, WiFi.localIP(), co2);
  // reset GPS fix state to detect lost signal.
  gps_got_fix = false;
  delay(min( (10 - (timeinfo.tm_sec % 10)) * 1000 - (millis() - start), (long unsigned int) 10*1000));
  // delay((60 - timeinfo.tm_sec) * 1000 - (millis() - start));
}


void update_time_gps() {
  static char gps_cmd_buf[1024];
  size_t n = GPS_UART.readBytesUntil('\n', gps_cmd_buf, 1024);

  if (n == 0) return;

  GPZDA_Data time_data = parse_gps_time_data(gps_cmd_buf, n);
  // Serial.print("GPS Status: "); Serial.println(time_data.status);

  switch(time_data.status) {
    case NOT_GPZDA: return;
    case OK_WITH_CHECKSUM: break;
    case OK_NO_CHECKSUM: break;
    case ERR_NO_GPS: break;
    default:
      Serial.print("GPZDA parse error: ");
      Serial.println(time_data.status);
      do_pick_time_source = true;
      break;
  }

  #if 0
  Serial.print("[");
  Serial.print(n);
  Serial.print("] Rx: ");
  for (int i = 0; i < n; i += 1) {
    Serial.print(gps_cmd_buf[i]);
  }
  Serial.print("\n");
  #endif

  gps_got_fix = (time_data.status != ERR_NO_GPS);
  if (!gps_got_fix) return;

  // prefer wifi time
  if (time_source == NTP) {
    return;
  }

  got_time_at_all = true;
  timeinfo.tm_hour = time_data.hour + gmt_offset_sec / 3600;
  timeinfo.tm_min = time_data.minute;
  timeinfo.tm_sec = time_data.second;

  struct timeval tv;
  tv.tv_sec = mktime(&timeinfo);
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);

  #if 0
  Serial.println("Recieved GPZDA data:");
  Serial.println(time_data.hour);
  Serial.println(time_data.minute);
  Serial.println(time_data.second);
  Serial.println(time_data.millisecond);
  Serial.println(time_data.day);
  Serial.println(time_data.month);
  Serial.println(time_data.year);
  Serial.println(time_data.timezone);
  Serial.println(time_data.timezone_min);
  Serial.println(time_data.checksum);
  Serial.println("END");
  #endif
}


void copy_screen_with_mask(int *dst, int *src, int *mask) {
  for (int x = 0; x < SCREEN_W; x += 1) {
    for (int y = 0; y < SCREEN_H; y += 1) {
      dst[SCREEN_W * y + x] = src[SCREEN_W * y + x] & mask[SCREEN_W * y + x];
    }
  }
}
