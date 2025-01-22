// PID Code:

#include <Wire.h>
#include <Servo.h>
#include <I2Cdev.h>
// #include <MPU6050.h>
#include "BMI088.h" //go through bmi088 

Servo myServoX;
Servo myServoY;

 myMPU;

int16_t Acc_rawX, Acc_rawY, Acc_rawZ; //Accel constants
int16_t Gyr_rawX, Gyr_rawY, Gyr_rawZ; //Gyro constants

float Acceleration_angleX;
float Acceleration_angleY;
float Gyro_angleX;
float Gyro_angleY;
float Total_angleX;
float Total_angleY;

float elapsedTime, time, timePrev;
float rad_to_deg = 180/3.141592654;

float PID_x, PID_y, error_x, error_y, previous_error_x, previous_error_y;
float pid_p_x=0;
float pid_p_y=0;
float pid_i_x=0;
float pid_i_y=0;
float pid_d_x=0;
float pid_d_y=0;

float kp=1.4; //Proportional constant
float ki=0.001; //Integral constant
float kd=0.01; //Derivative constant

float desired_angle = 0; //Desired TVC Angle
float ServoStartAngle = 90;

void setup() {
Wire.begin();
Wire.beginTransmission(0x68);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(250000);
myServoX.attach(9);
myServoY.attach (11);
myServoX.write (ServoStartAngle);
myServoY.write (ServoStartAngle);
time = millis(); //Start counting time in milliseconds
}

void loop() {
timePrev = time; // inital time stored before the actual time is read
time = millis(); // actual time
elapsedTime = (time - timePrev) / 1000;

Wire.beginTransmission(0x68);
Wire.write(0x3B); 
Wire.endTransmission(false);
Wire.requestFrom(0x68,6,true); 

Acc_rawX=Wire.read()<<8|Wire.read();
Acc_rawY=Wire.read()<<8|Wire.read();
Acc_rawZ=Wire.read()<<8|Wire.read();

/*X Euler Equations*/
Acceleration_angleX = atan((Acc_rawY/16384.0)/sqrt(pow((Acc_rawX/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
/*///Y Euler Equations///*/
Acceleration_angleY = atan(-1*(Acc_rawX/16384.0)/sqrt(pow((Acc_rawY/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
Wire.beginTransmission(0x68);
Wire.write(0x43);
Wire.endTransmission(false);
Wire.requestFrom(0x68,4,true);

Gyr_rawX=Wire.read()<<8|Wire.read();
Gyr_rawY=Wire.read()<<8|Wire.read();

/X Gyro Angle/
Gyro_angleX = Gyr_rawX/131.0;
/Y Gyro Angle/
Gyro_angleY = Gyr_rawY/131.0;

// /---X Axis Angle---/
Total_angleX = 0.98 (Total_angleX + Gyro_angleXelapsedTime) + 0.02Acceleration_angleX;
/---Y Axis Angle---/
Total_angleY = 0.98 (Total_angleY + Gyro_angleYelapsedTime) + 0.02Acceleration_angleY;

/Calculate the error between the desired angle and actual measured angle/
error_x = Total_angleX - desired_angle;
error_y = Total_angleY - desired_angle;

///Calculate proportional value/
pid_p_x = kperror_x;
pid_p_y = kperror_y;

/Calculate integral value/
if(-100 <error_x <100)
{
pid_i_x = pid_i_x+(ki*error_x);
}

if(-100 <error_y <100)
{
pid_i_y = pid_i_y+(ki*error_y);
}

// /Calculate derivative value/
pid_d_x = kd*((error_x - previous_error_x)/elapsedTime);
pid_d_y = kd*((error_y - previous_error_y)/elapsedTime);

// /PID value is the sum of each of this 3 parts/
PID_x = pid_p_x + pid_i_x + pid_d_x;
PID_y = pid_p_y + pid_i_y + pid_d_y;

Serial.println ("PID Value: ");
Serial.println (PID_x);
Serial.println (PID_y);

myServoX.write (ServoStartAngle - PID_x);
myServoY.write (ServoStartAngle - PID_y);

previous_error_x = error_x;
previous_error_y = error_y;//Remember to store the previous error.

}