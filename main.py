import numpy as np
import quaternion as qt
from rocket_math import PID
from rocket import Rocket
from components.gravity_component import GravityComponent
from components.tvc_component import SimpleMotor, TVCComponent, Null_TVCController, Gimbal
import components.tvc_controllers as tvc_controllers
from rocket_logger import RocketLogger  


# rocket with 100 meters elevation of mass of 5kg and ground plane of 0 starting at rest
pitch = np.deg2rad(20) # 20 degrees launch rail
initial_orientation =  qt.from_rotation_vector([0.0, np.pi/2 + pitch, 0.0])
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
simpleMotor = SimpleMotor(thrust=100)

nullTVCController = Null_TVCController(position=(0.05, 0.05))

Kp = 6.0
Kd = 2.0
Ki = 0.0
pidy = PID(Kp, Ki, Kd)
pidz = PID(Kp, Ki, Kd)

simplePIDController = tvc_controllers.SimplePIDController(pid_y=pidy, pid_z=pidz)

simpleGimbal = Gimbal((np.pi/2, np.pi/2), (0,0))
simpleTVCComponent = TVCComponent(simpleMotor, simpleGimbal, nullTVCController)

rocket.addComponent(simpleTVCComponent)

#making rocketLogger
filepath =  "TVC/rocketlogger/gravityRocketInitialVelocity.csv"
rl = RocketLogger(rocket, filepath)
#rl2 = RocketLogger(gravityRocket, "rocketlogger/gravityMotorRocketNew.csv")


dt = 0.002 #set step time
duration = 4 #duration
ground_hit_time = None
hit_ground = False

rl.open()

print("running sim!")
for i in range(0, int(duration/dt)):
    rl.record()
    rocket.step(dt)
    
    if dt*i*1000%10 == 0:
        print("running:", i*dt, 'seconds')

    #check ground hit time
    if rocket.x[2] <= 0.0 and not hit_ground:
        ground_hit_time = dt*i
        hit_ground = True

print("sim done!")
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
    body_axes_stride=5,        # body axes every n plots
    vec_length=2,
    body_axes_lengths={'x': 9},
    title="Rocket Flight – World Frame",
    flip_z=True
)
print("plotting done!")