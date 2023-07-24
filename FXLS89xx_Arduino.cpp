/*
 * Copyright 2023 ryraki
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file FXLS89xx_Arduino.cpp
 * @brief This file contains the functions for FXLS89xx Accelerometer evaluation by Arduino
 */
 
#include "FXLS89xx_Arduino.h"

FXLS89xx_Arduino::FXLS89xx_Arduino(uint8_t i2c_address) : I2C_device(i2c_address) {}
FXLS89xx_Arduino::FXLS89xx_Arduino(TwoWire& wire, uint8_t i2c_address) : I2C_device(wire, i2c_address) {}
FXLS89xx_Arduino::~FXLS89xx_Arduino() {}

//void FXLS89xx_Arduino::run() {
//  run(false);
//}

void FXLS89xx_Arduino::run(bool sdcd) {
  bit_op8(FXLS896x_SENS_CONFIG1, ~FXLS896x_SENS_CONFIG1_ACTIVE_MASK, 0);
  bit_op8(FXLS896x_SENS_CONFIG1, ~FXLS896x_SENS_CONFIG1_FSR_MASK, sensor_range<<FXLS896x_SENS_CONFIG1_FSR_SHIFT);
  bit_op8(FXLS896x_SENS_CONFIG2, ~(FXLS896x_SENS_CONFIG2_SLEEP_PM_MASK|FXLS896x_SENS_CONFIG2_WAKE_PM_MASK),
    (wake_pm<<FXLS896x_SENS_CONFIG2_WAKE_PM_SHIFT) | (sleep_pm<<FXLS896x_SENS_CONFIG2_SLEEP_PM_SHIFT));
  write_r8(FXLS896x_SENS_CONFIG3, (wake_odr<<FXLS896x_SENS_CONFIG3_WAKE_ODR_SHIFT) | sleep_odr);
  if (!sdcd)
    bit_op8(FXLS896x_INT_EN, ~FXLS896x_INT_EN_DRDY_EN_MASK, FXLS896x_INT_EN_DRDY_EN_EN);
  bit_op8(FXLS896x_SENS_CONFIG1, ~FXLS896x_SENS_CONFIG1_ACTIVE_MASK, FXLS896x_SENS_CONFIG1_ACTIVE_ACTIVE);
}

void FXLS89xx_Arduino::sdcd(ref_updm ref, XYZ xyz, float upper_threshold, float lower_threshold, uint8_t debounce) {
  // Set upper threshold
  uint16_t upper_threshold_12bit=upper_threshold/0.98/pow(2,sensor_range);
  if (upper_threshold>1024*pow(2,sensor_range+1)) upper_threshold_12bit=0x07ff;
  else if (upper_threshold<-1024*pow(2,sensor_range+1)) upper_threshold_12bit=0x0800;
  Serial.println(upper_threshold_12bit, HEX);
  
  // Set lower threshold
  uint16_t lower_threshold_12bit=lower_threshold/0.98/pow(2,sensor_range);
  if (lower_threshold>1024*pow(2,sensor_range+1)) lower_threshold_12bit=0x07ff;
  else if (lower_threshold<-1024*pow(2,sensor_range+1)) lower_threshold_12bit=0x0800;
  Serial.println(lower_threshold_12bit, HEX);

  // Set configuration of debounce counters and lower/upper thresholds
  uint8_t reg[6] = {debounce, debounce, lower_threshold_12bit&0xff, lower_threshold_12bit>>8, upper_threshold_12bit&0xff, upper_threshold_12bit>>8};
  reg_w(FXLS896x_SDCD_OT_DBCNT, reg, 6);

  uint8_t sdcd_conf[2] = {
    xyz<<3|xyz,
    ref<<FXLS896x_SDCD_CONFIG2_REF_UPDM_SHIFT | FXLS896x_SDCD_CONFIG2_SDCD_EN_EN
  };
  reg_w(FXLS896x_SDCD_CONFIG1, sdcd_conf, 2);
  Serial.println(sdcd_conf[1], HEX);

  // Route SDCD interruption to INT2
  bit_op8(FXLS896x_INT_EN,
    ~(FXLS896x_INT_EN_SDCD_WT_EN_MASK | FXLS896x_INT_EN_SDCD_OT_EN_MASK),
    sdcd_wt ? FXLS896x_INT_EN_SDCD_WT_EN_EN : FXLS896x_INT_EN_SDCD_OT_EN_EN);

  bit_op8(FXLS896x_INT_PIN_SEL,
    ~(FXLS896x_INT_PIN_SEL_SDCD_WT_INT2_MASK | FXLS896x_INT_PIN_SEL_SDCD_OT_INT2_MASK),
    sdcd_wt ? FXLS896x_INT_PIN_SEL_SDCD_WT_INT2_EN : FXLS896x_INT_PIN_SEL_SDCD_OT_INT2_EN);
}

void FXLS89xx_Arduino::enable_sleep(uint16_t asleep) {
  uint8_t aslp_reg[2] = {asleep&0xff, asleep>>8};
  reg_w(FXLS896x_ASLP_COUNT_LSB, aslp_reg, 2);

  bit_op8(FXLS896x_SENS_CONFIG4, ~(FXLS896x_SENS_CONFIG4_WK_SDCD_OT_MASK | FXLS896x_SENS_CONFIG4_WK_SDCD_WT_MASK),
    sdcd_wt ? FXLS896x_SENS_CONFIG4_WK_SDCD_WT_EN : FXLS896x_SENS_CONFIG4_WK_SDCD_OT_EN);
}

void FXLS89xx_Arduino::read_XYZ(float *pOutBuffer) {
  uint8_t output[6];
  reg_r(FXLS896x_OUT_X_LSB, output, 6);
  for (int i=0; i<3; i++) {
    if (output[2*i+1]&0xf8)
      pOutBuffer[i] = ((float)((((uint8_t)~output[2*i+1])&0x0f)<<8)+((uint8_t)~output[2*i])+1)*(-0.98)*(sensor_range+1);
    else pOutBuffer[i] = ((float)(((uint8_t)output[2*i+1]&0x0f)<<8)+(uint8_t)output[2*i])*0.98*(sensor_range+1);
  }
}

void FXLS89xx_Arduino::EXT_TRIG_init(bool pinconfig) {
  if (pinconfig)
    pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  bit_op8(FXLS896x_SENS_CONFIG1, ~FXLS896x_SENS_CONFIG1_ACTIVE_MASK, 0);
  bit_op8(FXLS896x_INT_EN, ~FXLS896x_INT_EN_DRDY_EN_MASK, FXLS896x_INT_EN_DRDY_EN_EN);
  bit_op8(FXLS896x_SENS_CONFIG4, 0, FXLS896x_SENS_CONFIG4_EXT_TRIG_M_MULTIPLE | FXLS896x_SENS_CONFIG4_INT2_FUNC_EXT_TRIG | FXLS896x_SENS_CONFIG4_INT_POL_ACT_HIGH);
}

void FXLS89xx_Arduino::EXT_TRIG_Trigger() {
  pinMode(A0, INPUT);
  delay(1);
  pinMode(A0, OUTPUT);  
}

uint8_t FXLS89xx_Arduino::init(bool pinconfig) {
  if (pinconfig) {
    pinMode(SDA, INPUT);
    pinMode(SCL, INPUT);
    pinMode(2, INPUT);
  }
  write_r8(FXLS896x_SENS_CONFIG1, FXLS896x_SENS_CONFIG1_RST_RST);
  while(!digitalRead(2));
  while(digitalRead(2));
  return read_r8(FXLS896x_WHO_AM_I);
}
