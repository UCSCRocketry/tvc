#include "motors/servo_drivers.h"
#include "config.h"
#include <Arduino.h>

#define DEFAULT_MINPOINT_US 500
#define DEFAULT_MAXPOINT_US 1500
#define DEFAULT_SERVO_RANGE_RADS PI



Servo_Axis::Servo_Axis(uint8_t servo_pin):
    servo(Servo())
{   
    this->servo_pin = servo_pin;
    servo_offset_rads = 0.0;
    ratio = 1.0;

    current_servo_angle_rads = 0.0;

    minpoint_us = DEFAULT_MINPOINT_US; // standard servo min pulse width
    maxpoint_us = DEFAULT_MAXPOINT_US; // standard servo max pulse widtih
    servo_range_rads = DEFAULT_SERVO_RANGE_RADS; // 180 degree range for standard servo
}

void Servo_Axis::attach(){
    servo.attach(servo_pin);
}

void Servo_Axis::set_offset(float offset_rads){
    this->servo_offset_rads = offset_rads;
}
void Servo_Axis::set_ratio(float ratio){
    this->ratio = ratio;
}

void Servo_Axis::set_min_max_range(int minpoint_us, int maxpoint_us, float servo_range_rads){
    this->minpoint_us = minpoint_us;
    this->maxpoint_us = maxpoint_us;
    this->servo_range_rads = servo_range_rads;
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



Gimbal::Gimbal(Servo_Axis& y_axis, Servo_Axis& z_axis) :
    axis_y(y_axis),
    axis_z(z_axis)
    {}

Gimbal::Gimbal(int y_pin, int z_pin) : 
    axis_y(Servo_Axis(y_pin)),
    axis_z(Servo_Axis(z_pin))
    {               }

void Gimbal::set_angles(float angle_y_rads, float angle_z_rads){
    axis_y.set_axis_angle(angle_y_rads);
    axis_z.set_axis_angle(angle_z_rads);
}

void Gimbal::drive_servos(){
    axis_y.drive_servo();
    axis_z.drive_servo();
}

void Gimbal::drive_servos(float angle_y_rads, float angle_z_rads){
    set_angles(angle_y_rads, angle_z_rads);
    drive_servos();
}

void Gimbal::setup(){
    axis_y.set_offset(SERVO_OFFSET_Y); axis_z.set_offset(SERVO_OFFSET_Z);
    axis_y.set_ratio(GIMBAL_RATIO); axis_z.set_ratio(GIMBAL_RATIO);
    axis_y.set_min_max_range(SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH, SERVO_RANGE_RADS);
    axis_z.set_min_max_range(SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH, SERVO_RANGE_RADS);

    axis_y.attach();
    axis_z.attach();
}