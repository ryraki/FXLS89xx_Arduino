#ifndef _FXLS89xx_Arduino_h
#define _FXLS89xx_Arduino_h

#include "fxls896x.h"

/*
 * Definition
 */

enum ESensorErrors
{
    SENSOR_ERROR_NONE = 0,
    SENSOR_ERROR_INVALID_PARAM,
    SENSOR_ERROR_BAD_ADDRESS,
    SENSOR_ERROR_INIT,
    SENSOR_ERROR_WRITE,
    SENSOR_ERROR_READ,
};

enum SensorRange 
{
  _2G = 0,
  _4G,
  _8G,
  _16G
};

typedef struct
{
    uint16_t writeTo; /* Address where the value is writes to.*/
    uint8_t value;    /* value. Note that value should be shifted based on the bit position.*/
    uint8_t mask;     /* mask of the field to be set with given value.*/
} registerwritelist_t;
#define __END_WRITE_DATA__ {.writeTo=0xFFFF, .value=0}

/*
 * Functions
 */
int16_t FXLS89xx_I2C_Read(uint8_t slaveAddress, uint16_t offset, uint8_t len, uint8_t *pOutBuffer);
int16_t FXLS89xx_I2C_Read_XYZ(uint8_t slaveAddress, float *pOutBuffer, uint8_t *output, uint8_t range);
int16_t FXLS89xx_I2C_Write_Single(uint8_t slaveAddress, uint16_t writeTo, uint8_t value, uint8_t mask);
int16_t FXLS89xx_I2C_Write(uint8_t slaveAddress, const registerwritelist_t *pRegWriteList);
int16_t FXLS89xx_I2C_EXT_TRIG_Init(uint8_t sAddress);
int16_t FXLS89xx_EXT_TRIG_Trigger();
int16_t FXLS89xx_I2C_Initialize(uint8_t sAddress, uint8_t *whoami);

#endif