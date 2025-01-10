void altitude_control() {
  // Set the desired altitude (example: 10 ft)
  altitude_setpoint = 10.0;

  // Update the altitude input from the sensor
  altitude_input = altitude;

  // Compute the PID output
  altitude_pid.Compute();

  // Apply the output (e.g., adjust EDF power or control surfaces)
  // For example: analogWrite(EDF_motor_pin, altitude_output);
}

void attitude_control() {
  // Set the desired attitude (0º for level flight)
  attitude_setpoint = 0.0;

  // Update the attitude input from the sensor
  attitude_input = attitude;

  // Compute the PID output
  attitude_pid.Compute();

  // Apply the output (e.g., adjust EDF power or control surfaces)
  // For example: analogWrite(attitude_control_pin, attitude_output);
}