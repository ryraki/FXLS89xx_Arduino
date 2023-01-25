/*
 * Copyright 2023 ryrak
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file example_EXT_TRIG.ino
 * @brief This file can measure G value by using EXT_TRIG function
 */
#include <Wire.h>
#include <FXLS89xx_Arduino.h>

/*
 * Definition for test functions
 */
#define I2C_ADDR FXLS896x_DEVICE_ADDRESS_SA0_0

/*
 * Functions
 */
void setup() {
  // put your setup code here, to run once:
  // Initialize the device and take the WHO_AM_I parameter
  Serial.begin(9600);
  uint8_t whoami=0;
  int16_t stat = FXLS89xx_I2C_Initialize(I2C_ADDR, &whoami);
  switch (whoami) {
    case FXLS8962_WHOAMI_VALUE:
      Serial.println("FXLS8962AF");
      break;
    case FXLS8964_WHOAMI_VALUE:
      Serial.println("FXLS8964AF");
      break;
    case FXLS8967_WHOAMI_VALUE:
      Serial.println("FXLS8967AF");
      break;
    case FXLS8968_WHOAMI_VALUE:
      Serial.println("FXLS8968CF");
      break;
    case FXLS8974_WHOAMI_VALUE:
      Serial.println("FXLS8974CF");
      break;
    default:
      Serial.println("???");
      break;
  }
  stat = FXLS89xx_I2C_EXT_TRIG_Init(I2C_ADDR);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Repeated every 1 sec
  FXLS89xx_EXT_TRIG_Trigger();  // Triggers EXT_TRIG
  while (!digitalRead(2));      // Wait until D2 (INT1) rises                                    
  uint8_t output[6];
  float float_output[3];
  FXLS89xx_I2C_Read_XYZ(I2C_ADDR, float_output, output, _2G); // Read and calculate XYZ G Data 
  
  // Outputs calculated XYZ G Data and raw G data onto serial
  Serial.print("X: ");
  Serial.print(float_output[0]);
  Serial.print("mg, ");
  Serial.println(((output[1]&0x0f)<<8)|output[0], HEX);
  Serial.print("Y: ");
  Serial.print(float_output[1]);
  Serial.print("mg, ");
  Serial.println(((output[3]&0x0f)<<8)|output[2], HEX);
  Serial.print("Z: ");
  Serial.print(float_output[2]);
  Serial.print("mg, ");
  Serial.println(((output[5]&0x0f)<<8)|output[4], HEX);
  delay(1000);
}
