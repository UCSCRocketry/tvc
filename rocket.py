# stores information about rocket class for use in simulation class

from dataclasses import dataclass, field
from physics_component import Component 

import numpy as np
from numpy import linalg
import rocket_math as rkm
import quaternion as qt


@dataclass # this automatically creates the constructor
class Rocket:
    
    #state  variables
    x: np.ndarray #world
    v: np.ndarray #world
    w_body: np.ndarray #body    
    q: qt.quaternion #body to world

    m: float 
    J_body: np.ndarray #body about CoM

    ground_elev: float | None = None

    # accumulators
    F_body: np.ndarray = field(default_factory=lambda: np.zeros(3))
    F_world: np.ndarray = field(default_factory=lambda: np.zeros(3))
    tau_body: np.ndarray = field(default_factory=lambda: np.zeros(3))

    components: list = field(default_factory=list[Component])
    
    global_time: float = 0.0 # time from start of simulation

    F_net_world: np.ndarray = field(default_factory=lambda: np.zeros(3))
    
    last_dt: float = 0.0

    # adds a physics component to the components list
    def addComponent(self, component : Component):
        self.components.append(component)
    
    def applyForceBody(self, f_body):
        self.F_body += f_body
    def applyForceWorld(self, f_world):
        self.F_world += f_world
    def applyTorqueBody(self, tau_body):
        self.tau_body += tau_body
    



    #ran each step of the simulation. requires to be sent delta time from previous step in seconds
    # for realtime simulation, dt is calculated on runtime
    # for non-realtime simulation, dt can be a small, constant step where smaller dt means greater frequency
    def step(self, dt: float):
        #calculate forces and moments
        self.F_body[:] = 0.0
        self.F_world[:] = 0.0
        self.tau_body[:] = 0.0

        # applies each of the components, which will in turn update the forces and moments acting on the rocket.
        for c in self.components:
            c.apply(self, dt) 
        
        #calculate linear acceleration
        F_net_world = self.F_world +   qt.as_rotation_matrix(self.q) @ self.F_body  
        self.F_net_world = F_net_world
        #v_dot_body = self.F_body / self.m
        v_dot_world = F_net_world/self.m
        
        #calculate angular acceleration
        # someone should double-check this math
        Jw = self.J_body @ self.w_body
        gyro = np.cross(self.w_body, Jw)
        w_dot = np.linalg.solve(self.J_body, (self.tau_body - gyro))

        #calculate linear velocity
        
        self.v += v_dot_world * dt

        #calculate angular velocity
        self.w_body += w_dot * dt

        #calculate new position
        self.x += self.v * dt 

        #check if ground elev has been defined and set accordingly
        if self.ground_elev is not None: 
            if self.x[2] > self.ground_elev:
                self.x[2] = self.ground_elev
                if self.v[2] > 0:
                    self.v[2] = 0


        #calculate new orientation
        #calc new quaternion d_q from d_theta and multiply to original quaternion
        '''
        i dont know what I was trying to do here im ngl 

        w_body_mag = linalg.norm(self.w_body)
        w_body_norm = self.w_body.copy()
        if w_body_mag > 0:
            w_body_norm /= w_body_mag
        '''
        
        d_theta = self.w_body * dt  
        #dq = qt.from_euler_angles(d_theta) okay it's clearly not euler angles
        dq = qt.from_rotation_vector(d_theta)

        #calculate new body to world quaternion and normalize
        
        # dq_w = np.cos(d_theta / 2.0)
        # dq_r = (self.w / w_norm) * np.sin(d_theta / 2.0)

        self.q = (self.q * dq)
        n = np.abs(self.q)
        if n > 0:
            self.q /= n

        self.global_time += dt
        self.last_dt = dt
        