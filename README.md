This is the rocket sim for a thrust vectoring system 


how this thing works:
    simulation.py (not yet implmented) (idk what it'll end up being used for)
        in the simulation class, we run the simulation. any updates that occur mid-simulation can be induced there. this is not yet implemented
    rocket.py: 
        contains the rocket dataclass which stores the basic rocket state variables, including position and velocity. 
        it also stores the Components of the rocket
        
        the rocket class contains the step function which runs each component and updates the rocket accordingly
    physics_component.py:
        contains the abstract class Component, which all other physics components inherits.
        Components modify the forces and moments applied to the rocket
    gravity_component.py:
        so far, the only functional component. 
        this applies a constant downward force proportional to the rocket's mass
    rocket_math.py:
        stores any useful and specialized math that isn't included in an import
    
    the other classes have not yet been impelmeneted


conventions:

axis:
    body reference frame:
        +X forward or roll, +Y right or pitch, +Z down or yaw
        (on a rocket launchpad, x will point directly vertical)
    world reference frame:
        +Z for altitude

units:
    meters, kg, seconds, rads by default



rocket.py : 
    contains dataclass rocket
    
    instance variables:
        np arrays
            x: position (xyz) (world) (meters) 
            v: velocity (xyz) (world) (meters/second)
            w: angular velocity (xyz) (body) (rad/sec)
            q: body -> world (quaternion)

            J: inertia tensor (3x3) (body) 

        accumulators:
            F_body 
            tau_body
            components

        floats:
            m: mass (kg)
        
    functions:
        


