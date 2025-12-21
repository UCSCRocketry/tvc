from rocket_math import PID 
import matplotlib.pyplot as plt

#basic pid implementation

kp=1.2
ki=0.4
kd=0.1

pid = PID(kp, ki, kd)
t = 0 
dt = 0.01

#process variable
pv = 0
#setpoint 
sp = 100

pv_data = []
t_data = []

while t < 100:  
    pv_data.append(pv)
    t_data.append(t)

    u = pid.control(sp, pv, dt)
    pv += u * dt  

    t += dt

plt.plot(t_data,pv_data)
plt.show()
