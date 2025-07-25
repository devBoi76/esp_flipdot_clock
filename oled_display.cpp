#include "oled_display.hpp"
#include "config.hpp"

bool try_init_oled(TwoWire &wire) {
  Serial.println("[OLED] init...");

  if (!wire.begin(OLED_SDA, OLED_SCL)) return false;

  oled = Adafruit_SSD1306(OLED_W, OLED_H, &wire, OLED_RESET);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) return false;
  
  oled.display();
  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println(F("Display Init success."));
  oled.println(F("Waiting for WiFi or GPS..."));
  oled.display();

  return true;
}

void update_oled(bool got_time_at_all, bool gps_got_fix, TimeSource time_source, IPAddress ip_addr, float co2_ppm) {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.print(F("IP: "));
  oled.println(ip_addr.toString());

  oled.setTextSize(2);
  oled.print(F("Time: "));
  if (got_time_at_all) {
    switch (time_source) {
      case GPS: oled.println(F("GPS")); break;
      case NTP: oled.println(F("NTP")); break;
      case LOCAL: oled.println(F("ESP")); break;
    }
  } else {
    oled.println(F("NONE"));
  }
  oled.print(F("FIX: "));
  if (gps_got_fix) {
    oled.println(F("YES"));
  } else {
    oled.println(F("NO"));
  }

  oled.print(F("CO2: "));
  oled.print((int) co2_ppm);
  oled.display();
}
