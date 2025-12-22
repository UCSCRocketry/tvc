from abc import ABC, abstractmethod
from rocket import Rocket

from .tvc_component import TVCController, TVCCommand
from rocket_math import PID
import numpy as np
import quaternion as qt
'''
simple pid: a simple, 2-pid driven tvc controller

uses 2 pid loops; one for each axis
calculates the tilt off vertical along the axes of the rocket 



'''

class SimplePIDController(TVCController):
    def __init__(self, pid_y: PID, pid_z: PID):
        self.pidy = pid_y
        self.pidz = pid_z

    def get_command(self,rocket : Rocket,forces, moments,  dt):
        world_to_body_transform = np.transpose(qt.as_rotation_matrix(rocket.q))
        
        u = world_to_body_transform @ np.array([0,0,-1])
        u_x = u[0]
        u_y = u[1]
        u_z = u[2]

        s = np.sqrt(u_z**2 + u_y**2)
        c = u_x

        eps = 1e-8

        if s < eps:
            y_error = -u_z   # pitch error about +y
            z_error =  u_y   # yaw error about +z
        else:
            theta = np.arctan2(s, c)
            gain = theta / s
            y_error = -u_z * gain
            z_error =  u_y * gain


        y = self.pidy.control(0, y_error, dt)
        z = self.pidz.control(0, z_error, dt)
        
        command = TVCCommand(y,z, 1.0)
        return command
