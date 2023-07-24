/*
 * Copyright 2023 ryraki
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file example_SDCD_fixed.ino
 * @brief This file can utilize SDCD (fixed threshold) function to detect 0.7g>Z>0.3g
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
  pinMode(A0, INPUT);
  Wire.begin();
  
  uint8_t whoami = fxls89xx.init();
  Serial.print("WHO_AM_I: ");
  Serial.println(whoami, HEX);
  fxls89xx.wake_odr = FXLS89xx_ODR_6_25HZ;
  fxls89xx.wake_pm  = FXLS89xx_HPM;
  fxls89xx.sensor_range = FXLS89xx_RANGE_2G;
  fxls89xx.sdcd(FXLS89xx_ABSOLUTE, FXLS89xx_Z, false, 700, 300);
  fxls89xx.run();
}

void loop() {
  while (!digitalRead(2));      // Wait until D2 (INT1) rises                                    
  float float_output[3];
  fxls89xx.read_XYZ(float_output); // Read and calculate XYZ G Data
  Serial.print("SDCD_INT(Z):");
  Serial.print(digitalRead(A0)?1000:0);
  Serial.print(",Z:");
  Serial.println(float_output[2]);
}
