#ifndef DYNAMICS_H_
#define DYNAMICS_H_

#include "matrix.h"

namespace controls {
class Dynamics {
  public:
  enum States {
    kX = 0,
    kY = 1,
    kZ = 2,

    kXdot = 3,
    kYdot = 4,
    kZdot = 5,

    kQuatX = 6,
    kQuatY = 7,
    kQuatZ = 8,
    kQuatW = 9,

    kOmegaX = 10,
    kOmegaY = 11,
    kOmegaZ = 12,

    // External torques and forces
    kFX = 13,
    kFY = 14,
    kFZ = 15,

    kTX = 16,
    kTY = 17,
    kTZ = 18,
    kNumStates = 19,
  };

  enum ErrorStates {
    kDX = 0,
    kDY = 1,
    kDZ = 2,

    kDXdot = 3,
    kDYdot = 4,
    kDZdot = 5,
    
    kDThetaX = 6,
    kDThetaY = 7,
    kDThetaZ = 8,

    kDOmegaX = 9,
    kDOmegaY = 10,
    kDomegaZ = 11,

    kDFX = 12,
    kDFY = 13,
    kDFZ = 14,

    kDTX = 15,
    KDTY = 16,
    KDTZ = 17,
    kNumErrorStates = 18,
  };

  enum Inputs {
    kWProp = 0,
    kTrw = 1,
    kTheta = 2,
    kPsi = 3,
    kNumInputs = 4,
  };

  struct DynamicsConstants {
    float32_t mass;
    float32_t r;

    Matrix<3, 3> inertia;
  };

  typedef Matrix<kNumStates, 1> State;
  typedef Matrix<kNumInputs, 1> Input;

  Dynamics(DynamicsConstants constants);

  private:
    State f(State X, Input U);

    float32_t prop_thrust(float32_t omega);

    DynamicsConstants constants_;
};
};

#endif // DYNAMICS_H_
