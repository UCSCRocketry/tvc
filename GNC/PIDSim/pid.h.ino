#include <PID_v1.h>

// Define the PID variables for altitude and attitude
double altitude_input, altitude_output, altitude_setpoint;
double attitude_input, attitude_output, attitude_setpoint;

// PID tuning parameters for altitude control (these values can be adjusted)
double Kp_alt = 1.0, Ki_alt = 0.1, Kd_alt = 0.05;

// PID tuning parameters for attitude control
double Kp_att = 1.0, Ki_att = 0.1, Kd_att = 0.05;

// Create the PID controllers
PID altitude_pid(altitude_input, altitude_output, altitude_setpoint, Kp_alt, Ki_alt, Kd_alt, DIRECT);
PID attitude_pid(attitude_input, attitude_output, attitude_setpoint, Kp_att, Ki_att, Kd_att, DIRECT);

// Function to initialize PID controllers
void init_PID() {
  altitude_pid.SetMode(AUTOMATIC);
  attitude_pid.SetMode(AUTOMATIC);
}

