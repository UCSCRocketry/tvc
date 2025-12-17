from dataclasses import dataclass
import numpy as np

#includes any useful functions and classes

'''
lookup_table_2way class

stores a lookup table 
basically an interpolated dictionary
-- currently uses linear interpolation, may add new interpolation methods later. should work fine though? 
can get value from a key and get a key from a value.
'''
@dataclass
class lookup_table_2way:
    def __init__(self, key_value_dict:dict[float, float]):
        self.keys = sorted(key_value_dict.keys())
        self.values = [key_value_dict[key] for key in self.keys]
    
    def get_value(self, key):
        

'''
PID class
Implementation of basic PID control loop.

psuedocode - source: wikipedia
previous_error := 0
integral := 0
loop:
   error := setpoint − measured_value
   proportional := error;
   integral := integral + error × dt
   derivative := (error - previous_error) / dt
   output := Kp × proportional + Ki × integral + Kd × derivative
   previous_error := error
   wait(dt)
   goto loop
'''

@dataclass
class PID:
    # pid constants
    k_p : float
    k_i : float
    k_d : float
    
    prev_error : float = 0.0
    integral : float = 0.0 

    # one step of the PID loop
    def control(
        self,
        setpoint : float, # target value  
        process_value : float, # current value 
        dt : float # delta time (seconds, usually)
    ):
        
        error = setpoint - process_value
        #pid terms
        proportional = error
        self.integral += error * dt
        derivative = (error - self.prev_error) / dt
        #calculate output
        output = self.k_p * proportional + self.k_i * self.integral + self.k_d * derivative
        self.prev_error = error

        return output
