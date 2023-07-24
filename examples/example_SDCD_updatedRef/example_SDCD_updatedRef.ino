/*
 * Copyright 2023 ryraki
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file example_SDCD_updatedRef.ino
 * @brief SDCD function is utilized with sleep mode. Please check with Serial Plotter. When Z crosses upper or lower threshold, the device wakes from SLEEP mode. When 5 measurements are done without crossing thresholds, the device goes to SLEEP.
 */
#include "FXLS89xx_Arduino.h"

/*
 * Definition for test functions
 */

FXLS89xx_Arduino fxls89xx;
float upper_threshold=100;
float lower_threshold=-100;
float previousZ;

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
  fxls89xx.wake_odr = FXLS89xx_ODR_12_5HZ;
  fxls89xx.wake_pm  = FXLS89xx_HPM;
  fxls89xx.sleep_odr = FXLS89xx_ODR_1_563HZ;
  fxls89xx.sleep_pm  = FXLS89xx_LPM;
  fxls89xx.sensor_range = FXLS89xx_RANGE_2G;
  fxls89xx.sdcd_wt = false;
  fxls89xx.sdcd(FXLS89xx_DIFF_PREVIOUS, FXLS89xx_Z, upper_threshold, lower_threshold);
  fxls89xx.enable_sleep(5);
  fxls89xx.run();
}

void loop() {
  while (!digitalRead(2));      // Wait until D2 (INT1) rises                                    
  float float_output[3];
  fxls89xx.read_XYZ(float_output); // Read and calculate XYZ G Data
  Serial.print(",Z:");
  Serial.print(float_output[2]);
  Serial.print(",Upper_Threshold:");
  Serial.print(previousZ+upper_threshold);
  Serial.print(",Lower_Threshold:");
  Serial.print(previousZ+lower_threshold);
  Serial.print(",SDCD_INT(Z):");
  Serial.println(digitalRead(A0)?1000:0);
  previousZ=float_output[2];
}
