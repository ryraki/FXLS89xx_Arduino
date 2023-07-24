# FXLS89xx_Arduino
This library enables the developer to evaluate NXP FXLS89xx with Arduino.

## Warning
Make sure that **J6-2 MUST NOT be connected** when you use this library.  
Since the IOREF pin on Arduino is supplied by 5V, **shorting IOREF (5V) and +3V3 (3.3V) can cause either FXLS89xx board or Arduino board damaged.**  
This driver offers all functions by only I2C communication because all Arduino output H signal is 5V.
<img src="J6-2.JPG" width="320px" alt="Cut J6-2 in order NOT to short 5V and 3.3V"/>

## Datasheet and Board Information
Device Information|Datasheet|Board Information
---|---|---
[FXLS8961AF](https://www.nxp.com/products/sensors/accelerometers/2g-4g-8g-16g-low-power-12-bit-digital-accelerometer:FXLS8961AF)|[FXLS8961AF.pdf](https://www.nxp.com/docs/en/data-sheet/FXLS8961AF.pdf)|[FRDM-STBA-A8961](https://www.nxp.com/design/software/sensor-toolbox/evaluation-boards/sensor-toolbox-development-board-for-fxls8961af-3-axis-accelerometer:FRDM-STBA-A8961)
[FXLS8964AF](https://www.nxp.com/products/sensors/accelerometers/2g-4g-8g-16g-low-power-12-bit-digital-accelerometer:FXLS8964AF)|[FXLS8964AF.pdf](https://www.nxp.com/docs/en/data-sheet/FXLS8964AF.pdf)|[FRDM-STBA-A8964](https://www.nxp.com/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8964af-3-axis-accelerometer:FRDM-STBA-A8964)
[FXLS8967AF](https://www.nxp.com/products/sensors/accelerometers/2g-4g-8g-16g-low-power-12-bit-digital-accelerometer:FXLS8967AF)|[FXLS8967AF.pdf](https://www.nxp.com/docs/en/data-sheet/FXLS8967AF.pdf)|[FRDM-STBA-A8967](https://www.nxp.com/design/software/development-software/sensor-toolbox-sensor-development-ecosystem/sensor-toolbox-development-board-for-fxls8967af-3-axis-accelerometer:FRDM-STBA-A8967)
[FXLS8971CF](https://www.nxp.com/products/sensors/accelerometers/2g-4g-8g-16g-low-power-12-bit-digital-accelerometer:FXLS8971CF)|[FXLS8971CF.pdf](https://www.nxp.com/docs/en/data-sheet/FXLS8971CF.pdf)|[FRDM-STBI-A8971](https://www.nxp.com/design/software/sensor-toolbox/evaluation-boards/sensor-toolbox-development-board-for-fxls8971cf-3-axis-iot-accelerometer:FRDM-STBI-A8971)|
[FXLS8974CF](https://www.nxp.com/products/sensors/accelerometers/2g-4g-8g-16g-low-power-12-bit-digital-iot-accelerometer:FXLS8974CF)|[FXLS8974CF.pdf](https://www.nxp.com/docs/en/data-sheet/FXLS8974CF.pdf)|[FRDM-K22F-A8974/FRDM-STBI-A8974](https://www.nxp.com/design/software/sensor-toolbox/evaluation-boards/sensor-toolbox-development-board-for-fxls8974cf-3-axis-iot-accelerometer:FRDM-K22F-A8974)
