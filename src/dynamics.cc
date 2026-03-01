#include "dynamics.h"

#include "lie.h"

using controls::Dynamics;
typedef controls::Dynamics::States States;
typedef controls::Dynamics::Inputs Inputs;
typedef controls::Dynamics::State State;
typedef controls::Dynamics::Input Input;

using lie::Quaternion;

Dynamics::Dynamics(Dynamics::DynamicsConstants constants):
  constants_(constants) {}

State Dynamics::f(State X, Input U) {
  const Matrix<3, 1> lift_prop({0.0, 0.0, prop_thrust(U[Inputs::kWProp])});

  Quaternion prop_body_quat = Quaternion::from_zyx(0.0, U[Inputs::kTheta], U[Inputs::kPsi]);
  
  auto lift_body = prop_body_quat.apply(lift_prop);

  const Matrix<3, 1> r({0.0, 0.0, constants_.r});

  // T = Matrix<3,1>::cross(r, lift_body) + U[Inputs::kTrw] + Matrix<3, 1>{0.0, 0.0, self.prop_torque()};
}

float32_t Dynamics::prop_thrust(float32_t omega) {
  return 0.000020759 * omega*omega - 0.00605731 * omega + 1.12302;
}
