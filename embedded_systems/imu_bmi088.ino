#include "BMI088.h"

// i2c object declaration
Bmi088Accel accel(Wire,0x18);

// spi object declaration 
Bmi088Accel accel(SPI,10);
