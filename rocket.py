# stores information about rocket class for use in simulation class


# stores center of gravity, thrust, as position vector from base of rocket 


class Rocket:

    def __init__(self):
        self.mass #mass in kilograms, float
        self.CoG #center of gravity; coordinates in meters, float
        self.CoT #center of thrust; coordinates in meters, float

        self.motor #motor model from motor class

    
    def induceForce(self, force):
        pass #induces force on rocket 

    def calcMoments(self):
        pass

    def calcForces(self):
        pass

    def updatePosition(self):
        #velocity verlet alg 
        pass