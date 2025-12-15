import numpy as np
import quaternion

from rocket import Rocket
from gravity_component import Gravity_Component

# rocket with 100 meters elevation of mass of 5 and ground plane of 0 starting at rest
gravityRocket = Rocket(np.array([0.0,0.0,100.0]), np.array([0.0,0.0,0.0]), np.array([0.0,0.0,0.0]), np.quaternion(1.0,0.0,0.0,0.0), 5.0, np.identity(3), 0.0)
# gravity component
gravity = Gravity_Component()
# applying gravity to rocket
gravityRocket.addComponent(gravity)

#if this works correctly, it should take 4.515 seconds to land
dt = 0.001 #set step time
rocket_pos = []
record_period = 10 #record every n cycles
ground_hit_time = None
hit_ground = False
t = 0
for i in range(0,5000):
    if i % record_period == 0:
        rocket_pos.append(gravityRocket.x)
    gravityRocket.step(dt)
    if gravityRocket.x[2] <= 0.0 and not hit_ground:
        ground_hit_time = dt*i
        hit_ground = True
    t += dt

print(ground_hit_time)
# running the simulation gives us 4.514 seconds