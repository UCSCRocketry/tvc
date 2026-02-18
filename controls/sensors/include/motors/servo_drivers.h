#ifndef SERVO_DRIVERS_H
#define SERVO_DRIVERS_H

#include <Servo.h>
#include "config.h"
#include <Arduino.h>


class Servo_Axis{
public:
    Servo_Axis(uint8_t servo_pin);
    void set_offset(float offset_rads);
    void set_ratio(float ratio);
    void set_min_max_range(int minpoint_us, int maxpoint_us, float servo_range_rads);
    void set_servo_angle(float angle_rads);
    void set_axis_angle(float angle_rads);
    void drive_servo();
    void attach();
private:
    Servo servo; 
    float servo_offset_rads;
    float ratio;
    float current_servo_angle_rads;

    int minpoint_us;
    int maxpoint_us;
    float servo_range_rads;
    int servo_pin; 
};

class Gimbal{ 
public:
    Gimbal(Servo_Axis& y_axis, Servo_Axis& z_axis);
    Gimbal(int y_pin, int z_pin);

    void set_angles(float angle_y_rads, float angle_z_rads);
    void drive_servos();
    void drive_servos(float angle_y_rads, float angle_z_rads);
    void setup();
    
private:
    Servo_Axis axis_y;
    Servo_Axis axis_z;
    
};


#endif