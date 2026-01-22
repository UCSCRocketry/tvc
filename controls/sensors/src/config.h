#ifndef CONFIG_H
#define CONFIG_H

// hardware setup: 

// bno055 imu 
#define BNO055_I2C_ADDRESS BNO055_ADDRESS_A // default i2c address for bno055
#define BNO055_WIRE &Wire

// bmp388 barometric pressure sensor
#define BMP388_I2C_ADDRESS 0x76 // default i2c address for bmp388
#define BMP388_WIRE &Wire

// environmental setup:
#define SEA_LEVEL_PRESSURE_HPA (1013.25) // local sea level pressure






#endif