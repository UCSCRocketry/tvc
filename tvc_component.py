### all of this is out of date with how it's currently implemented. read README and other files to understand.
class Motor:

    def __init__(self, 
                 thrustCurve, # lookup table, from lookupTable.py (in Newtons)
                 gimbalPosition, #3d vector, from center of mass of rocket (meters)
                 maxGimbalAngle, #2d vector (rad)
                 maxGimbalRate, #2d vector (rad/s^2)
                 ):
        self.thrustCurve = thrustCurve
        self.angularMomentumCurve

        
        self.targetAngle = 0 

    def vectorTo(self): #vector to angle, in rads. Takes time to vector, probably?
        pass
    
    def throttleTo(self): #throttle to thrust percentage (0-100). Takes time to spool, probably?
        pass

    def calcForces(self):
        pass
