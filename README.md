# Thrust Vector Control (TVC) Rocket Simulation

This is the rocket sim for a thrust vectoring system.

---

## Project Structure

### `simulation.py`
*(not yet implemented)*  
*(idk what it'll end up being used for)*

- In the `Simulation` class, we run the simulation.
- Any updates that occur mid-simulation can be induced there.
- This is not yet implemented.

---

### `rocket.py`

Contains the `Rocket` dataclass which stores the basic rocket state variables, including position and velocity.  
It also stores the `Components` of the rocket.

- The `Rocket` class contains the `step` function which runs each component and updates the rocket accordingly.

---

### `physics_component.py`

Contains the abstract class `Component`, which all other physics components inherit.

- Components modify the forces and moments applied to the rocket.

---

### `gravity_component.py`

So far, the only functional component.

- This applies a constant downward force proportional to the rocket's mass.

---

### `rocket_math.py`

Stores any useful and specialized math that isn't included in an import.

---

### Other Files

- The other classes have not yet been implemented.

---

## Conventions

### Axis Definitions

#### Body Reference Frame
- `+X` forward or roll  
- `+Y` right or pitch  
- `+Z` down or yaw  

*(On a rocket launchpad, X will point directly vertical)*

#### World Reference Frame
- `+Z` for altitude

---

### Units

- meters  
- kilograms  
- seconds  
- radians  

*(meters, kg, seconds, rads by default)*

---

## `rocket.py`

Contains the `Rocket` dataclass.

---

### Instance Variables

#### NumPy Arrays

- `x`  
  - position (xyz)  
  - world frame  
  - meters  

- `v`  
  - velocity (xyz)  
  - world frame  
  - meters/second  

- `w`  
  - angular velocity (xyz)  
  - body frame  
  - rad/sec  

- `q`  
  - body → world  
  - quaternion  

- `J`  
  - inertia tensor (3×3)  
  - body frame  

---

#### Accumulators

- `F_body`
- `tau_body`
- `components`

---

#### Floats

- `m`  
  - mass (kg)

---

### Functions

*(to be implemented)*

