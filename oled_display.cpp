#include "oled_display.hpp"

bool try_init_oled(TwoWire &wire) {
  Serial.println("[OLED] init...");

  if (!wire.begin(5, 4)) return false;

  oled = Adafruit_SSD1306(OLED_W, OLED_H, &wire, OLED_RESET);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) return false;
  
  oled.display();
  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println(F("Display Init success."));
  oled.display();

  return true;
}

void update_oled(bool got_time_at_all, bool gps_got_fix, TimeSource time_source, IPAddress ip_addr, float co2_ppm) {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.print("IP: ");
  oled.println(ip_addr.toString());

  oled.setTextSize(2);
  oled.print("Czas: ");
  if (got_time_at_all) {
    switch (time_source) {
      case GPS: oled.println("GPS"); break;
      case NTP: oled.println("NTP"); break;
      case LOCAL: oled.println("ESP"); break;
    }
  } else {
    oled.println("BRAK");
  }

  oled.print((int) co2_ppm);
  oled.print("ppmCO2");
  oled.display();
}