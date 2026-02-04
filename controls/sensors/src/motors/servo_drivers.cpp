#include "motors/servo_drivers.h"
#include "config.h"
#include <Arduino.h>


Servo_Axis::Servo_Axis(uint8_t servo_pin){
    servo = Servo();
    servo.attach(servo_pin);
    
    servo_offset_rads = 0.0;
    ratio = 1.0;

    current_servo_angle_rads = 0.0;

    minpoint_us = 500; // standard servo min pulse width
    maxpoint_us = 1500; // standard servo max pulse widtih
    servo_range_rads = PI; // 180 degree range for standard servo
}
void Servo_Axis::set_offset(float offset_rads){
    this->servo_offset_rads = offset_rads;
}
void Servo_Axis::set_ratio(float ratio){
    this->ratio = ratio;
}

void Servo_Axis::set_servo_angle(float angle_rads){
    //clip servo_angle to [-servo_range_rads/2, servo_range_rads/2]
    float new_angle = fmax(fmin(angle_rads, servo_range_rads/2), -servo_range_rads/2);
    current_servo_angle_rads = new_angle;
}

void Servo_Axis::set_axis_angle(float angle_rads){
    float servo_angle = angle_rads / ratio + servo_offset_rads;
    set_servo_angle(servo_angle);
}

void Servo_Axis::drive_servo(){
    // map the current_servo_angle_rads to pulse width
    float us_per_rads = float(maxpoint_us - minpoint_us ) / servo_range_rads;
    int pulse_width_us = int(minpoint_us + (current_servo_angle_rads) * us_per_rads);
    servo.writeMicroseconds(pulse_width_us);
}


Gimbal::Gimbal(uint8_t servo_pin_y, uint8_t servo_pin_z) : 
    axis_y(Servo_Axis(servo_pin_y)), 
    axis_z(Servo_Axis(servo_pin_z)) 
    {}

Gimbal::Gimbal(Servo_Axis y_axis, Servo_Axis z_axis) :
    axis_y(y_axis),
    axis_z(z_axis)
    {}