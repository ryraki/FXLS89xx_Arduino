/*
 * Copyright 2023 ryraki
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file FXLS89xx_Arduino.h
 * @brief This file contains the header for FXLS89xx Accelerometer evaluation by Arduino
 */
#ifndef _FXLS89xx_Arduino_h
#define _FXLS89xx_Arduino_h

#include <Arduino.h>
#include "fxls896x.h"
#include <I2C_device.h>

  enum odr_freq {
    FXLS89xx_ODR_3200HZ = ((uint8_t)0x00),
    FXLS89xx_ODR_1600HZ = ((uint8_t)0x01),
    FXLS89xx_ODR_800HZ = ((uint8_t)0x02),
    FXLS89xx_ODR_400HZ = ((uint8_t)0x03),
    FXLS89xx_ODR_200HZ = ((uint8_t)0x04),
    FXLS89xx_ODR_100HZ = ((uint8_t)0x05),
    FXLS89xx_ODR_50HZ = ((uint8_t)0x06),
    FXLS89xx_ODR_25HZ = ((uint8_t)0x07),
    FXLS89xx_ODR_12_5HZ = ((uint8_t)0x08),
    FXLS89xx_ODR_6_25HZ = ((uint8_t)0x09),  /*  6.25 HZ                                         */
    FXLS89xx_ODR_3_125HZ = ((uint8_t)0x0a), /*  3.125 HZ                                        */
    FXLS89xx_ODR_1_563HZ = ((uint8_t)0x0b), /*  1.563 HZ                                        */
    FXLS89xx_ODR_0_781HZ = ((uint8_t)0x0c) /*  0.781 HZ                                        */
  };

  enum SensorRange 
  {
    FXLS89xx_RANGE_2G = 0,
    FXLS89xx_RANGE_4G,
    FXLS89xx_RANGE_8G,
    FXLS89xx_RANGE_16G
  };
  enum PowerMode
  {
    FXLS89xx_LPM=0, FXLS89xx_HPM, FXLS89xx_FPM
  };
  enum XYZ
  {
    FXLS89xx_Z=1,
    FXLS89xx_Y,
    FXLS89xx_YZ,
    FXLS89xx_X,
    FXLS89xx_XZ,
    FXLS89xx_XY,
    FXLS89xx_XYZ
  };
  enum ref_updm {
    FXLS89xx_UPDATED_REF=0,    //!< Reference value is updated when SDCD_OT_EA flag transitions from false to true (SDCD interruption occurs). Reference+/-threshold is compared with acceleration value.
    FXLS89xx_FIXED_REF,     //!< Reference value is fixed when SDCD function is enabled or REF_UPD bit is set. Reference+/-threshold is compared with acceleration value.
    FXLS89xx_DIFF_PREVIOUS, //!< Delta between current and previous value is compared with threshold.
    FXLS89xx_ABSOLUTE       //!< Reference is fixed and absolute acceleration value is compared with threshold.
  };

/**
* @brief Class to evaluate FXLS89xx
*/
class FXLS89xx_Arduino : public I2C_device{
  public:
    SensorRange sensor_range = FXLS89xx_RANGE_2G;   //!< @brief Sensor range selected through 2g,4g,8g,16g
    PowerMode wake_pm = FXLS89xx_LPM;         //!< @brief Power Mode in WAKE state selected from LPM, HPM, FPM
    PowerMode sleep_pm = FXLS89xx_LPM;        //!< @brief Power Mode in SLEEP state selected from LPM, HPM, FPM
    odr_freq wake_odr = FXLS89xx_ODR_100HZ;   //!< @brief ODR in WAKE state
    odr_freq sleep_odr = FXLS89xx_ODR_12_5HZ; //!< @brief ODR in SLEEP state
    bool sdcd_wt=false;                       //!< @brief true when interruption happens inside thresholds, false when outside thresholds

    /**
    * @brief Read XYZ accelerometer value with unit milligravity
    * @param pOutBuffer Pointer to array to store register contents
    */
    void read_XYZ(float *pOutBuffer);

    /**
    * @brief Start conversion based on the settings of sensor_range, wake_pm, sleep_pm, wake_odr, sleep_odr
    * @param sdcd If you set SDCD mode (using SLEEP states), then put true here.
    */
    void run(bool sdcd=false);

    /**
    * @brief Set SDCD function
    * @param ref_updm
    * @param upper_threshold Upper threshold by milligravity
    * @param lower_threshold Lower threshold by milligravity
    * @param debounce Debounce counter threshold value set to both upper and lower threshold
    */
    void sdcd(ref_updm ref, XYZ xyz, float upper_threshold, float lower_threshold, uint8_t debounce=0);

    /**
    * @brief Sleep mode enable
    * @param asleep Asleep count threshold: The device goes to SLEEP state when count reaches to this threshold value. The count increments when no interruption occurs.
    */
    void enable_sleep(uint16_t asleep=0);

    /**
    * @brief Initialize the pin configurations and register for EXT_TRIG mode
    * @param pinconfig If you wouldn't like to config the pins by this function, put false in this parameter. This parameter can be skipped.
    */
    void EXT_TRIG_init(bool pinconfig=true);

    /**
    * @brief Trigger the pin to start one-time conversion. Make sure once EXT_TRIG init is done first.
    */
    void EXT_TRIG_Trigger();

    /**
    * @brief Initialize the device with reset
    * @param pinconfig If you wouldn't like to config the pins by this function, put false in this parameter. This parameter can be skipped.
    * @return WHO_AM_I register value
    */
    uint8_t init(bool pinconfig=true);

    /** Create a FXLS89xx_Arduino instance
    * @param i2c_address If the device is configured with SA0=H, put FXLS896x_DEVICE_ADDRESS_SA0_1.
    */
    FXLS89xx_Arduino(uint8_t i2c_address=FXLS896x_DEVICE_ADDRESS_SA0_0);
    
    /** Create a FXLS89xx_Arduino instance
    * @param wire TwoWire instance
    * @param i2c_address If the device is configured with SA0=H, put FXLS896x_DEVICE_ADDRESS_SA0_1.
    */
    FXLS89xx_Arduino(TwoWire& wire, uint8_t i2c_address=FXLS896x_DEVICE_ADDRESS_SA0_0);

  /** Destructor of FXLS89xx_Arduino
   */
  virtual ~FXLS89xx_Arduino();

  #if DOXYGEN_ONLY
    /** Get temperature value in degree Celsius [°C] 
    *
    *	This method simply calls "temp()" method	
    *
    * @return temperature value in degree Celsius [°C] 
    */
    virtual float read( void );

    /** Ping the device
    *
    * @return true when ACK 
    */
    bool ping( void );
      
    /** Multiple register write
    * 
    * @param reg register index/address/pointer
    * @param data pointer to data buffer
    * @param size data size
    * @return transferred data size
    */
    int reg_w( uint8_t reg_adr, uint8_t *data, uint16_t size );

    /** Single register write
    * 
    * @param reg register index/address/pointer
    * @param data pointer to data buffer
    * @param size data size
    * @return transferred data size
    */
    int reg_w( uint8_t reg_adr, uint8_t data );

    /** Multiple register read
    * 
    * @param reg register index/address/pointer
    * @param data pointer to data buffer
    * @param size data size
    * @return transferred data size
    */
    int reg_r( uint8_t reg_adr, uint8_t *data, uint16_t size );

    /** Single register read
    * 
    * @param reg register index/address/pointer
    * @return read data
    */
    uint8_t	reg_r( uint8_t reg_adr );

    /** Register write, 8 bit
    *
    * @param reg register index/address/pointer
    * @param val data value
    */
    void write_r8( uint8_t reg, uint8_t val );

    /** Register write, 16 bit
    * 
    *	This 16 bit access may ot be common but it's useful for sensor devices
    *
    * @param reg register index/address/pointer
    * @param val data value
    */
    void write_r16( uint8_t reg, uint16_t val );

    /** Register read, 8 bit
    *
    * @param reg register index/address/pointer
    * @return data value
    */
    uint8_t read_r8( uint8_t reg );

    /** Register read, 16 bit
    *	
    *	This 16 bit access may ot be common but it's useful for sensor devices
    *
    * @param reg register index/address/pointer
    * @return data value
    */
    uint16_t read_r16( uint8_t reg );

    /** Register overwriting with bit-mask
    *	
    *	Register can be updated by bit level
    *
    * @param reg register index/address/pointer
    * @param mask bit-mask to protect overwriting
    * @param value value to overwrite
    */
    void bit_op8(  uint8_t reg,  uint8_t mask,  uint8_t value );
    void bit_op16( uint8_t reg, uint16_t mask, uint16_t value );
  #endif	// DOXYGEN_ONLY
};

#endif