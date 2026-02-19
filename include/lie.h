#ifndef LIE_H_
#define LIE_H_

#include "matrix.h"
#include "math.h"

// Defines operators and classes for SO(3)
namespace lie {

// Scalar last quaternion
class Quaternion {
  public:
    Quaternion(float32_t quat[4]);

    explicit Quaternion(const std::array<float32_t, 4> values);

    Quaternion(const Matrix<3, 1> vector, float32_t scalar);

    Quaternion(const Quaternion &other);

    // Returns the scalar part of the quaternion
    float32_t scalar() const;

    // Returns the vector part of the quaternion
    const Matrix<3, 1> vector() const;
    
    // Applies the rotation of the quaternion to a vector p
    Matrix<3, 1> apply(const Matrix<3, 1> &p) const;

    static Quaternion from_zyx(float32_t roll, float32_t pitch, float32_t yaw);

    // Quaternion composition aka the quaternion product
    static Quaternion compose(Quaternion a, Quaternion b);

  protected:
    float32_t quat_[4];
};

namespace operators {
// R^3 -> SO(3)
Quaternion exp(Matrix<3, 1> phi);

// SO(3) -> R^3
Matrix<3, 1> log(Quaternion phi);

// Defines the Gamma operator which is the jacobian of exponential map.
// Also defined as the right jacobian in a lot of literature
Matrix<3, 3> gamma(Matrix<3, 1> phi);

// This is just the skew operator, doesn't need to be defined in lie::operators but right now thats the only place its used.
Matrix<3, 3> skew(Matrix<3, 1> vector);
} // operators

} // lie

#endif // LIE_H_
