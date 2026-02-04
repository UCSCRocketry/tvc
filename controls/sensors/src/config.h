#ifndef CONFIG_H
#define CONFIG_H

// useful constants
#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)
#define GRAVITY 9.80665 // m/s^2

// hardware setup: 

//servo settings
#define SERVO_PIN_Y 9
#define SERVO_PIN_Z 10
#define SERVO_OFFSET_Y 0 // radians
#define SERVO_OFFSET_Z 0 // radians
#define GIMBAL_RATIO 1.0 // output to input gear ratio (rad/rad or unitless)
#define SERVO_MIN_PULSE_WIDTH 1000 // in microseconds
#define SERVO_MAX_PULSE_WIDTH 2000 // in microseconds
#define SERVO_RANGE_RADS (PI) // radians, total range of motion for servo


// bno055 imu 
#define BNO055_I2C_ADDRESS BNO055_ADDRESS_A // default i2c address for bno055
#define BNO055_WIRE &Wire

// bmp388 barometric pressure sensor
#define BMP388_I2C_ADDRESS 0x76 // default i2c address for bmp388
#define BMP388_WIRE &Wire

// environmental setup:
#define SEA_LEVEL_PRESSURE_HPA (1013.25) // local sea level pressure






#endif