/*
 * Copyright 2023 ryraki
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file example_DRDY_read.ino
 * @brief This file can measure XYZ accelerometer values in WAKE mode
 */
#include "FXLS89xx_Arduino.h"

/*
 * Definition for test functions
 */

FXLS89xx_Arduino fxls89xx;

/*
 * Functions
 */
void setup() {
  // put your setup code here, to run once:
  // Initialize the device and take the WHO_AM_I parameter
  Serial.begin(115200);
  while (!Serial);
  Wire.begin();

  uint8_t whoami = fxls89xx.init();
  Serial.print("WHO_AM_I: ");
  Serial.println(whoami, HEX);
  fxls89xx.wake_odr = FXLS89xx_ODR_6_25HZ;
  fxls89xx.wake_pm  = FXLS89xx_HPM;
  fxls89xx.sensor_range = FXLS89xx_RANGE_2G;
  fxls89xx.run();
}

void loop() {
  while (!digitalRead(2));      // Wait until D2 (INT1) rises                                    
  float float_output[3];
  fxls89xx.read_XYZ(float_output); // Read and calculate XYZ G Data
  Serial.print("x:");
  Serial.print(float_output[0]);
  Serial.print(", y:");
  Serial.print(float_output[1]);
  Serial.print(", z:");
  Serial.println(float_output[2]);
}
