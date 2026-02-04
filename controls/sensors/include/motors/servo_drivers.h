#ifndef SERVO_DRIVERS_H
#define SERVO_DRIVERS_H

#include <Servo.h>
#include "config.h"


class Servo_Axis{
public:
    Servo_Axis(uint8_t servo_pin);
    void set_offset(float offset_rads);
    void set_ratio(float ratio);
    void set_servo_angle(float angle_rads);
    void set_axis_angle(float angle_rads);
    void drive_servo();
private:
    Servo servo; 
    float servo_offset_rads;
    float ratio;
    float current_servo_angle_rads;

    int minpoint_us;
    int maxpoint_us;
    float servo_range_rads;
};

class Gimbal{ 
public:
    Gimbal(uint8_t servo_pin_y, uint8_t servo_pin_2);
    Gimbal(Servo_Axis y_axis, Servo_Axis z_axis);
    Servo_Axis* get_y_axis();
    Servo_Axis* get_z_axis();

    void set_angles(float angle_y_rads, float angle_z_rads);
    void drive_servos();
    void drive_servos(float angle_y_rads, float angle_z_rads);
private:
    Servo_Axis axis_y;
    Servo_Axis axis_z;
};


#endif