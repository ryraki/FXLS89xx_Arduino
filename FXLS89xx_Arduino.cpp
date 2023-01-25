/*
 * Copyright 2023 ryrak
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file FXLS89xx_Arduino.cpp
 * @brief This file contains the functions for FXLS89xx Accelerometer evaluation by Arduino
 */
 
#include "Arduino.h"
#include "Wire.h"
#include "fxls896x.h"
#include "FXLS89xx_Arduino.h"

// Read Function
// slaveAddress: I2C slave address, either FXLS896x_DEVICE_ADDRESS_SA0_0 (0x18) or FXLS896x_DEVICE_ADDRESS_SA0_1 (0x19)
// offset: First register address
// len: length to read
// pOutBuffer: Pointer for buffer that the read results are stored
int16_t FXLS89xx_I2C_Read(uint8_t slaveAddress, uint16_t offset, uint8_t len, uint8_t *pOutBuffer) {
  Wire.beginTransmission(slaveAddress);
  Wire.write(offset);
  Wire.endTransmission(false);
  Wire.requestFrom(slaveAddress, len);
  int read_num = 0;
  while (Wire.available()) {
    pOutBuffer[read_num] = Wire.read();
    read_num++;
  }
  if (read_num != len)
    return SENSOR_ERROR_READ;
  return SENSOR_ERROR_NONE;
}

int16_t FXLS89xx_I2C_Read_XYZ(uint8_t slaveAddress, float *pOutBuffer, uint8_t *output, uint8_t range) {
  //uint8_t output[6];
  FXLS89xx_I2C_Read(slaveAddress, FXLS896x_OUT_X_LSB, 6, output);
  for (int i=0; i<3; i++) {
    if (output[2*i+1]&0xf0)
      pOutBuffer[i] = ((float)((((uint8_t)~output[2*i+1])&0x0f)<<8)+((uint8_t)~output[2*i])+1)*(-0.98);
    else pOutBuffer[i] = ((float)(((uint8_t)output[2*i+1]&0x0f)<<8)+(uint8_t)output[2*i])*0.98;
  }
  return 0;
}

int16_t FXLS89xx_I2C_Write_Single(uint8_t slaveAddress, uint16_t writeTo, uint8_t value, uint8_t mask) {
  uint8_t config_val = 0;
  uint16_t stat = 0;
  if (mask) {
    stat = FXLS89xx_I2C_Read(slaveAddress, writeTo, 1, &config_val);
    if (stat != SENSOR_ERROR_NONE) return SENSOR_ERROR_READ;
    config_val = (config_val & mask) | (value & ~mask);
  } else
    config_val = value;
  Wire.beginTransmission(slaveAddress);
  Wire.write(writeTo);
  Wire.write(config_val);
  stat = Wire.endTransmission();
  if (stat != 0) return SENSOR_ERROR_WRITE;
  return SENSOR_ERROR_NONE;
}

// Write Function
int16_t FXLS89xx_I2C_Write(uint8_t slaveAddress, const registerwritelist_t *pRegWriteList) {
  const registerwritelist_t *pCmd = pRegWriteList;
  bool repeatedStart;
  int16_t stat = 0;
  do {
    const uint8_t mask = pCmd->mask;
    repeatedStart = (pCmd+1)->writeTo != 0xFFFF;
    stat = FXLS89xx_I2C_Write_Single(slaveAddress, pCmd->writeTo, pCmd->value, pCmd->mask);
    if (stat != SENSOR_ERROR_NONE) return SENSOR_ERROR_WRITE;
    ++pCmd;
  } while (repeatedStart);
  return SENSOR_ERROR_NONE;
}

int16_t FXLS89xx_I2C_EXT_TRIG_Init(uint8_t sAddress) {
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  registerwritelist_t pRegWriteList[] = {
    {FXLS896x_SENS_CONFIG1, 0, ~FXLS896x_SENS_CONFIG1_ACTIVE_MASK},
    {FXLS896x_INT_EN, FXLS896x_INT_EN_DRDY_EN_EN, ~FXLS896x_INT_EN_DRDY_EN_MASK},
    {FXLS896x_SENS_CONFIG4, FXLS896x_SENS_CONFIG4_EXT_TRIG_M_MULTIPLE | FXLS896x_SENS_CONFIG4_INT2_FUNC_EXT_TRIG | FXLS896x_SENS_CONFIG4_INT_POL_ACT_HIGH, 0},
    __END_WRITE_DATA__};
  int16_t stat = FXLS89xx_I2C_Write(sAddress, pRegWriteList);
  if (stat != SENSOR_ERROR_NONE) return stat;
  return SENSOR_ERROR_NONE;
}

int16_t FXLS89xx_EXT_TRIG_Trigger() {
  pinMode(A0, INPUT);
  delay(1);
  pinMode(A0, OUTPUT);  
  return SENSOR_ERROR_NONE;
}

// Initializes FXLS89xx
// !!!!!Please execute this function first just once!!!!!
int16_t FXLS89xx_I2C_Initialize(uint8_t sAddress, uint8_t *whoami) {
  Wire.begin();
  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);
  pinMode(2, INPUT);
  uint8_t reg = 0;
  uint8_t stat = FXLS89xx_I2C_Write_Single(sAddress, FXLS896x_SENS_CONFIG1, FXLS896x_SENS_CONFIG1_RST_RST, 0);
  while(!digitalRead(2));
  while(digitalRead(2));
  stat = FXLS89xx_I2C_Read(sAddress, FXLS896x_WHO_AM_I, 1, &reg);
  (*whoami) = reg;
  // Check if the device is either FXLS89xx
  if (stat)
    if ((FXLS8962_WHOAMI_VALUE != reg) || (FXLS8964_WHOAMI_VALUE != reg) || (FXLS8967_WHOAMI_VALUE != reg) || (FXLS8968_WHOAMI_VALUE != reg) || (FXLS8974_WHOAMI_VALUE != reg)) 
      return SENSOR_ERROR_BAD_ADDRESS;
    else return SENSOR_ERROR_INIT;
  return SENSOR_ERROR_NONE;
}
