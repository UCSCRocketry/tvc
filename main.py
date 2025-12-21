import numpy as np
import quaternion

from rocket import Rocket
from components.gravity_component import GravityComponent
from components.tvc_component import SimpleMotor, TVCComponent, Null_TVCController, Gimbal
from rocket_logger import RocketLogger  

# rocket with 100 meters elevation of mass of 5kg and ground plane of 0 starting at rest
initial_orientation = quaternion.quaternion((0.02 + np.sqrt(2)/2),0,np.sqrt(2)/2,0 )
initial_position = np.zeros(3)
initial_velocity = np.zeros(3)
initial_angular_rate = np.zeros(3)
mass = 1
inertia_tensor = np.identity(3) * mass
ground_level = 0.0

rocket = Rocket(initial_position, 
                initial_velocity, 
                initial_angular_rate, 
                initial_orientation, 
                mass, 
                inertia_tensor, 
                ground_level)

# gravity component
gravity = GravityComponent()
# applying gravity to rocket
rocket.addComponent(gravity)

#making simple motor, tvc controller
simpleMotor = SimpleMotor(100, 1)
simpleTVCController = Null_TVCController(position=(0.1, 0.1))
simpleGimbal = Gimbal((np.pi/2, np.pi/2), (0,0))
simpleTVCComponent = TVCComponent(simpleMotor, simpleGimbal, simpleTVCController)

rocket.addComponent(simpleTVCComponent)

#making rocketLogger
filepath =  "TVC/rocketlogger/gravityRocketInitialVelocity.csv"
rl = RocketLogger(rocket, filepath)
#rl2 = RocketLogger(gravityRocket, "rocketlogger/gravityMotorRocketNew.csv")


dt = 0.01 #set step time
duration = 5 #duration
ground_hit_time = None
hit_ground = False

rl.open()

for i in range(0, int(duration/dt)):
    rl.record()
    rocket.step(dt)
    
    #check ground hit time
    if rocket.x[2] <= 0.0 and not hit_ground:
        ground_hit_time = dt*i
        hit_ground = True


rl.close()


from rocket_grapher import graph3d

graph3d(
    filepath,
    decimate_hz=15,          
    show_trajectory=True,
    show_velocity=False,
    show_force=False,
    body_axes=("x"),
    vec_stride=2,              # vectors every n plots
    body_axes_stride=1,        # body axes every n plots
    vec_length=2,
    body_axes_lengths={'x': 3},
    title="Rocket Flight – World Frame",
    flip_z=True
)

