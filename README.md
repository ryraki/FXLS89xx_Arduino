# FXLS89xx_Arduino
This library enables the developer to evaluate NXP FXLS89xx with Arduino.

## Caution when using library
Make sure that **J6-2 MUST NOT be connected** when you use this library.  
Since the IOREF pin on Arduino is supplied by 5V, **shorting IOREF (5V) and +3V3 (3.3V) can cause either FXLS89xx board or Arduino board damaged.**  
This driver offers all functions by only I2C communication because all Arduino output H signal is 5V.
<img src="J6-2.JPG" width="320px" alt="Cut J6-2 in order NOT to short 5V and 3.3V"/>

## Functions
Please take a look at the example (example_EXT_TRIG).  
Call FXLS89xx_I2C_Initialize() function first before using any functions in this library. For using EXT_TRIG mode, call FXLS89xx_I2C_EXT_TRIG_Init() after the initialization function.  
Any I2C read and write can be done by FXLS89xx_I2C_Read() and FXLS89xx_I2C_Write_Single(). If you would like to write multiple bytes to the device, then you can use FXLS89xx_I2C_Write(). The function needs a pointer to registerwritelist_t, which is the list of the same argument of FXLS89xx_I2C_Write_Single (writeTo: register address, value: register value, mask: put bits you would like to mask. 0 to ignore the mask). Make sure that registerwritelist_t structure should have \_\_END_WRITE_DATA\_\_ on the last item whose writeTo=0xFFFF indicates the list end.

## Board Information
FXLS8964AF: https://www.nxp.jp/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8964af-3-axis-accelerometer:FRDM-STBA-A8964  
FXLS8967AF: https://www.nxp.jp/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8967af-3-axis-accelerometer:FRDM-STBA-A8967  
FXLS8974CF: https://www.nxp.jp/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8974cf-3-axis-iot-accelerometer:FRDM-K22F-A8974
