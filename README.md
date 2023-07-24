# FXLS89xx_Arduino
This library enables the developer to evaluate NXP FXLS89xx with Arduino.

## Warning
Make sure that **J6-2 MUST NOT be connected** when you use this library.  
Since the IOREF pin on Arduino is supplied by 5V, **shorting IOREF (5V) and +3V3 (3.3V) can cause either FXLS89xx board or Arduino board damaged.**  
This driver offers all functions by only I2C communication because all Arduino output H signal is 5V.
<img src="J6-2.JPG" width="320px" alt="Cut J6-2 in order NOT to short 5V and 3.3V"/>

## Board Information
FXLS8964AF: https://www.nxp.jp/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8964af-3-axis-accelerometer:FRDM-STBA-A8964  
FXLS8967AF: https://www.nxp.jp/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8967af-3-axis-accelerometer:FRDM-STBA-A8967  
FXLS8974CF: https://www.nxp.jp/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8974cf-3-axis-iot-accelerometer:FRDM-K22F-A8974
