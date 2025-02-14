#include "scd30_interface.hpp"
#include <Arduino.h>


void print_scd30_error(int16_t scd30_err, const char* tag) {
  static char scd30_error_msg[128];

  Serial.print(tag);
  Serial.print("  : SCD30 error: ");
  errorToString(scd30_err, scd30_error_msg, sizeof scd30_error_msg);
  Serial.println(scd30_error_msg);
}

bool try_init_scd30_sensor(TwoWire &wire) {

  Serial.println("[SCD30] Initializing sensor...");
  if (!wire.begin(SCD30_SDA, SCD30_SCL, 50000)) return false;

  scd30.begin(wire, SCD30_I2C_ADDR_61);
  scd30.stopPeriodicMeasurement();
  scd30.softReset();
  delay(2000);

  uint8_t major = 0;
  uint8_t minor = 0;

  scd30_err = scd30.readFirmwareVersion(major, minor);
  if (scd30_err != NO_ERROR) {
    print_scd30_error(scd30_err, "readFirmwareVersion()");
    return false;
  }
  Serial.print("Firmware version: ");
  Serial.print(major);
  Serial.print(".");
  Serial.println(minor);

  scd30_err = scd30.startPeriodicMeasurement(0);
  scd30.setMeasurementInterval(2);

  if (scd30_err != NO_ERROR) {
    print_scd30_error(scd30_err, "startPeriodicMeasurement()");
    return false;
  }
  return true;
}

bool poll_scd30_data(float &out_co2, float &out_temp, float &out_humid) {
  uint16_t data_ready;
  scd30_err = scd30.getDataReady(data_ready);
  if (scd30_err != NO_ERROR) {
    print_scd30_error(scd30_err, "getDataReady()");
    return false;
  }

  if (!data_ready) {
    return false;
  }

  scd30_err = scd30.readMeasurementData(out_co2, out_temp, out_humid);
  if (scd30_err != NO_ERROR) {
    print_scd30_error(scd30_err, "readMeasurementData()");
    return false;
  }

  return true;
}
