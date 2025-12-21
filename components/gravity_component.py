from abc import ABC
from physics_component import Component
from rocket import Rocket

import numpy as np


GRAVITATIONAL_ACCEL = 9.81 #m/s^2
GEOCENTRIC_GRAV_CONST = 398600441.8 * 10 ** 6 # (Earth mass) * m^3 * s−2
R_EARTH = 6.371 * 10 ** 6 

class GravityComponent(Component):
    def __init__(self, useUnivGravConst = False):
        self.using_universal_gravity = useUnivGravConst # i dont understand why you would ever use this but im adding this for fun 
        self.planet_mass = 1.0 # earth masses
        pass

    def apply(self, rocket : Rocket, dt: float):
        if self.using_universal_gravity:
            r = rocket.x[2]
            F_G = rocket.m * GEOCENTRIC_GRAV_CONST * self.planet_mass / ((r + R_EARTH)**2)
            rocket.applyForceWorld(np.array([0.0,0.0,F_G]))
        else:
            F_G = rocket.m * GRAVITATIONAL_ACCEL
            rocket.applyForceWorld(np.array([0.0,0.0,F_G]))
    
    def get_state_data(self):
        return dict()
    