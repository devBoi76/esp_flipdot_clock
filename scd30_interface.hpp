#pragma once

#include <SensirionI2cScd30.h>
#include "config.hpp"

// I2C SCD30 config
inline SensirionI2cScd30 scd30;
inline int16_t scd30_err;
inline bool scd30_init_success = false;
// #define SCD30_SDA 12
// #define SCD30_SCL 14
// #define SCD30_SDA 5
// #define SCD30_SCL 4


void print_scd30_error(int16_t scd30_err, const char* tag);

// https://github.com/Sensirion/arduino-i2c-scd30/blob/master/examples/exampleUsage/exampleUsage.ino
bool try_init_scd30_sensor(TwoWire &wire);

bool poll_scd30_data(float &out_co2, float &out_temp, float &out_humid);