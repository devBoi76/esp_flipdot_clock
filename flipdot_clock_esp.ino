#include <Preferences.h>

#include <Wire.h>

#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <WiFi.h>
#include <Arduino.h>
#include "esp_log.h"
#include <time.h>

#define SCREEN_H 9
#define SCREEN_W 28

#include "util.h"

#include "time_data.hpp"
#include "clock_display.hpp"
#include "gps_interface.hpp"
#include "scd30_interface.hpp"
#include "oled_display.hpp"
#include "flipdot_interface.hpp"
#include "pinouts.hpp"

// wifi config
// #define SSID_PREF "ssid"
// #define PASS_PREF "password"

bool wifi_set_correctly = false;
String wifi_ssid;
String wifi_password;
bool wifi_shoud_reconnect = true;

Preferences preferences;

const char *ntpServer = "pool.ntp.org";

const char *timezone_code_CET = "CET-1CEST,M3.5.0,M10.5.0/3";

// uart config
const int RXD1 = 22;
const int TXD1 = 23;

const int RXD2 = 13;
const int TXD2 = 0;
const int GPS_BAUD_RATE = 9600;
#define GPS_UART Serial2



// debug pin config
// #define DEBUG_LOG_ENABLE_PIN 16


// current co2, temp, and humidity
float co2 = -100;
float temperature = -100;
float humidity = -100;
bool scd30_data_valid = false;

enum DisplayedContent {
  TIME,
  TEMPERATURE,
  HUMIDITY,
  CO2,
};
DisplayedContent displayed_content = TIME;

// #define DISPLAY_CONTROL_PIN 36

DisplayedContent get_pressed_button() {
  const int volt_0 = 0;
  const int volt_0_55 = 350;
  const int volt_1_1 = 1000;
  const int volt_2_2 = 2000;
  const int volt_2_8 = 3000;

  int level = analogRead(DISPLAY_CONTROL_PIN);
  Serial.println(level);

  if (volt_0 <= level && level < volt_0_55) {
    Serial.println(level);
    return TIME;
  } else if (volt_0_55 <= level && level < volt_1_1) { // 0,55-1,1V - button 1
  Serial.println(level);
    return TEMPERATURE;
  } else if (volt_1_1 <= level && level < volt_2_2) { // 1,1-2,2V - button 2
  Serial.println(level);
    return HUMIDITY;
  } else if (volt_2_2 <= level && level < volt_2_8) { // 2,2-2,8V - button 3
  Serial.println(level);
    return CO2;
  } else {
    // keep the same
    return displayed_content;
  }
}

void on_gps_uart_rx();

// bool get_debug_log_enable() {
//   return digitalRead(DEBUG_LOG_ENABLE_PIN) == HIGH;
// }

bool try_connect_wifi(const char *ssid, const char *password) {
  if (!wifi_set_correctly) return false;

  if (wifi_shoud_reconnect == false && WiFi.isConnected()) return true;

  WiFi.disconnect();
  delay(100);

  Serial.print("[WiFi] SSID: ");
  Serial.println(ssid);
  Serial.print("[WiFi] Password: ");
  Serial.println(password);

  WiFi.begin(ssid, password);
  Serial.println("[WiFi] Connecting...");

  while (true) {

    switch(WiFi.status()) {
      case WL_CONNECTED:
        Serial.println("[WiFi] Connected");
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());
        wifi_shoud_reconnect = false;
        time_source = NTP;
        return true;
      case WL_NO_SSID_AVAIL:
        Serial.print("[Wifi] Error: wifi SSID not found:\t`");
        Serial.print(ssid);
        Serial.println('`');
        WiFi.disconnect();
        time_source = GPS;
        return false;
      case WL_CONNECT_FAILED:
      case WL_CONNECTION_LOST:
        Serial.println("[WiFi] Connection failed. retrying");
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

String new_ssid;
String new_password;
void on_recieve_computer_data() {
  if (!Serial.available()) return;

  String input = Serial.readStringUntil('\n');
  input.trim();

  if (input.length() == 0) {
    config_state = NONE;
    Serial.println("Empty input.");
    Serial.println("Type `config` to configure wifi.");
    return;
  }

  if (config_state == NONE) {
    // initial command recognition
    if (!input.startsWith("config")) {
        Serial.println("Unknown command. Type `config` to configure wifi.");
        return;
    }

    config_state = SSID;
    Serial.print("Enter wifi SSID (name): ");
  } else if (config_state == SSID) {
    new_ssid = input;
    config_state = PASSWD;
    Serial.println("Enter wifi password: ");
  } else if (config_state == PASSWD) {
    new_password = input;
    config_state = NONE;

    wifi_ssid = new_ssid;
    wifi_password = new_password;
    wifi_set_correctly = true;

    // save new credentials
    preferences.begin("esp-clock", false);

    preferences.putString(SSID_PREF, wifi_ssid);
    preferences.putString(PASS_PREF, wifi_password);

    preferences.end();

    do_pick_time_source = true;
    wifi_shoud_reconnect = true;
    // } else wifi_set_correctly = false;
  }


}

void init_wire() {
  // if (!Wire.begin(5, 4, 50000)) {
  //   Serial.println("[Wire] init failed.");
  // } else {
  //   Serial.println("[Wire] init success.");
  // }
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

void setup() {
  Serial.begin(115200);
  // init debug pin
  //pinMode(DEBUG_LOG_ENABLE_PIN, INPUT);


  // flipdot
  flipdot_init();
  
  // init screen
  screen_chars[n_screen_chars] = 0;
  init_cursor(&screen_cursor);

  Serial.println("ESP32 Flipdot display clock");

  // init preferences
  preferences.begin("esp-clock", false);

  wifi_ssid = preferences.getString(SSID_PREF);
  wifi_password = preferences.getString(PASS_PREF);

  preferences.end();

  if (!wifi_ssid.isEmpty() && !wifi_password.isEmpty())
    wifi_set_correctly = true;

  // init wifi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  time_source = try_connect_wifi(wifi_ssid.c_str(), wifi_password.c_str()) ? NTP : LOCAL;
  config_state = NONE;
  // disable wifi spam
  esp_log_level_set("wifi", ESP_LOG_NONE);

  // init gps
  GPS_UART.begin(GPS_BAUD_RATE, SERIAL_8N1, RXD2, TXD2);
  GPS_UART.onReceive(on_gps_uart_rx);
  Serial.onReceive(on_recieve_computer_data);

  Serial.println("[Serial] init success.");

  // init time
  setenv("TZ", timezone_code_CET, 1);
  tzset();

  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, ntpServer);
  sntp_init();

  if (WiFi.isConnected()) sntp_restart();

  // Init i2c bus for oled and scd30
  init_wire();
  // test screen
  Serial.println("[Screen test] begin");
  for (int i = 0; i < 3; i += 1) {
    if (check_error(put_string_animation(&screen_cursor, "||||||||||||||||||||||||||||", 30), "setup_1")) for(;;);
    delay(100);
    if (check_error(put_string_animation(&screen_cursor, "||||||||||||||||||||||||||||", 30, true), "setup_2")) for(;;);
  }
  Serial.println("[Screen test] end");
}

void resolve_time_data() {
  do_pick_time_source |= !WiFi.isConnected();

  if (do_pick_time_source) {
    if(try_connect_wifi(wifi_ssid.c_str(), wifi_password.c_str())) {
      time_source = NTP;
      do_pick_time_source = false;
      got_time_at_all = true;
    } else if (gps_got_fix) {
      time_source = GPS;
    } else {
      time_source = LOCAL;
    }
  }

  // gps handling code sets the localtime, so getLocalTime is accurate
  if (getLocalTime(&timeinfo)) {
    got_time_at_all = true;
  } else {
    slogln("Failed to obtain ntp time.");
    do_pick_time_source = true;
  }
}

void print_help() {
  slogln(">>> Type `config` to configure WiFi");
  slogln("    Then type in: wifi SSID, enter, wifi Password, enter");
}

// #define PHOTODIODE 39
// #define DISPLAY_TRESHOLD 300
void update_screen() {
  if (!wifi_set_correctly) { slogln("WiFi not configured. Type `configure`."); }

  slog("Current time source: ");
  if (time_source == GPS) slog("GPS\n");
  else if (time_source == NTP) slog("NTP\n");
  else if (time_source == LOCAL) slog("Device local time\n");

  slog("Got GPS fix?: ");
  slogln(gps_got_fix == true);

  int light_level = analogRead(PHOTODIODE); 
  
  if (light_level > 2000) { // 2.5V is high level. More than 2000 should be ok.
    slog("Light too low to update screen.");
    screen_chars[0] = ' ';
    screen_chars[1] = '(';
    screen_chars[2] = 'Z';
  } else {
    switch (displayed_content) {
    case TIME: {
      if (got_time_at_all == false) {
        screen_chars[0] = 'A';
        screen_chars[1] = 'G';
        screen_chars[2] = 'H';
      } else {
        if (timeinfo.tm_hour / 10 == 0) {
          screen_chars[0] = '_';
        } else {
          screen_chars[0] = '0' + (timeinfo.tm_hour / 10);
        }
        screen_chars[1] = '0' + (timeinfo.tm_hour % 10);
        screen_chars[2] = ':';
        screen_chars[3] = '0' + (timeinfo.tm_min / 10);
        screen_chars[4] = '0' + (timeinfo.tm_min % 10);
      }
      break;
    }
    case TEMPERATURE: {
      if (scd30_data_valid) {
        screen_chars[0] = '\'';
        screen_chars[1] = '\'';
        screen_chars[2] = '\'';
        screen_chars[3] = '\'';
        screen_chars[4] = '\'';
        itoa((int)temperature, screen_chars+5, 10);
        size_t n = strlen(screen_chars);
        // assume `temperature` isn't longer than 5 chars
        screen_chars[n] = 'C';
        screen_chars[n+1] = 0;
      } else {
        screen_chars[0] = '\'';
        screen_chars[1] = '\'';
        screen_chars[2] = '\'';
        screen_chars[3] = '\'';
        screen_chars[4] = '\'';
        screen_chars[5] = 'T';
        screen_chars[6] = 'e';
        screen_chars[7] = 'm';
        screen_chars[8] = 'p';
        screen_chars[9] = '.';
        screen_chars[10] = 0;
      }
      break;
    }
    case HUMIDITY: {
      if (scd30_data_valid) {
        screen_chars[0] = '\'';
        screen_chars[1] = '\'';
        screen_chars[2] = '\'';
        screen_chars[3] = '\'';
        screen_chars[4] = '\'';
        itoa((int)humidity, screen_chars+5, 10);
        size_t n = strlen(screen_chars);
        screen_chars[n] = '%';
        screen_chars[n+1] = 0;
      } else {
        screen_chars[0] = '\'';
        screen_chars[1] = '\'';
        screen_chars[2] = '\'';
        screen_chars[3] = '\'';
        screen_chars[4] = '\'';
        screen_chars[5] = '\'';
        screen_chars[6] = 'R';
        screen_chars[7] = 'H';
        screen_chars[8] = '%';
        screen_chars[9] = 0;
      }
      break;
    }
    case CO2: {
      // flip between both texts
      if (timeinfo.tm_sec % 10 < 5 && scd30_data_valid) {
        screen_chars[0] = '\'';
        screen_chars[1] = '\'';
        screen_chars[2] = '\'';
        // screen_chars[3] = '\'';
        // screen_chars[4] = '\'';
        itoa((int)co2, screen_chars+3, 10);
      } else {
        screen_chars[0] = '\'';
        screen_chars[1] = '\'';
        screen_chars[2] = '\'';
        screen_chars[3] = '\'';
        screen_chars[4] = '\'';
        screen_chars[5] = 'C';
        screen_chars[6] = 'O';
        screen_chars[7] = '2';
        screen_chars[8] = ':';
        screen_chars[9] = 0;
      }
      break;
    }
    }
  }

  clear_screen(&screen_cursor);

  int ret = put_string(&screen_cursor, screen_chars);
  check_error(ret, "update_screen()");

  set_mask(&screen_cursor, 1);
  print_screen(&screen_cursor);
}



unsigned long loop_pause_time_ms = 0;

void loop() {

  while (!got_time_at_all) {
    int ret;
    ret = put_string_animation(&screen_cursor, " AGH", 200);
    check_error(ret, "agh_anim_1");

    resolve_time_data();

    ret = put_string_animation(&screen_cursor, " AGH", 200, true);
    check_error(ret, "agh_anim_2");
    resolve_time_data();
  }

  while (millis() < loop_pause_time_ms) {
    // poll for screen change
    DisplayedContent new_content = get_pressed_button();
    if (new_content != displayed_content) {
      displayed_content = new_content;
      break;
    }

    delay(50);
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
  print_help();

  // reset GPS fix state to detect lost signal.
  gps_got_fix = false;
  loop_pause_time_ms = millis() + min( (5 - timeinfo.tm_sec % 5) * 1000, (int)5*1000);
}

void on_gps_uart_rx() {
  static char gps_cmd_buf[1024];
  size_t n = GPS_UART.readBytesUntil('\n', gps_cmd_buf, 1024);
  gps_cmd_buf[n] = 0;
  if (n == 0) return;

  GPSTimeUpdateData gps_data = parse_gps_message(gps_cmd_buf, n+1);
  if (gps_data.is_gpzda == false) return;

  gps_got_fix = gps_data.got_fix;

  // ignore - wifi is better
  if (time_source == NTP) return;
  
  do_pick_time_source |= (gps_got_fix == false || gps_data.time_data_valid == false) && time_source == GPS;

  // no time data available
  if (gps_data.time_data_valid == false) {
    Serial.println("[GPS] Data invalid!");
    return;
  }

  if (gps_got_fix == false) return;

  got_time_at_all = true;
  // update local time
  // TODO: Handle error?
  struct timeval tv;
  tv.tv_sec = my_timegm(&gps_data.utc_timeinfo);
  tv.tv_usec = 0;
  // if (get_debug_log_enable()) {
  //   Serial.print("[GPS debug] Timestamp: ");
  //   Serial.println((uint64_t)tv.tv_sec);
  // }
  settimeofday(&tv, NULL);
}
