#include "lie.h"

using lie::Quaternion;

constexpr float32_t epsilon = 1e-4;

Quaternion lie::operators::exp(Matrix<3, 1> phi) {
  float32_t norm = phi.norm();

  if (norm < epsilon) {
    return Quaternion({phi.at(0, 0) / 2.0f, phi.at(1, 0) / 2.0f, phi.at(2, 0) / 2.0f, 1.0f});
  }

  float32_t scalar = cos(norm / 2.0f);

  Matrix<3, 1> vector = sin(norm / 2.0f) * (phi / norm);

  return Quaternion(vector, scalar);
}

Matrix<3, 1> lie::operators::log(Quaternion phi) {
  const Matrix<3, 1> vector = phi.vector();
  float32_t norm = vector.norm();

  if (norm < epsilon) {
    float32_t sign = std::copysign(1, phi.scalar());

    return sign * vector;
  }

  return 2 * atan2(vector.norm(), phi.scalar()) * (vector / vector.norm());
}

Matrix<3, 3> lie::operators::gamma(Matrix<3, 1> phi) {
  float32_t norm = phi.norm();

  Matrix<3, 3> skew_phi = lie::operators::skew(phi);

  if (norm < epsilon) {
    return Matrix<3, 3>::Identity() + (0.5f * skew_phi);
  }

  return Matrix<3, 3>::Identity() + ((1 - cos(norm)) * skew_phi) / (norm * norm) + ((norm - sin(norm)) * skew_phi * skew_phi) / (norm * norm * norm);
}

Matrix<3, 3> lie::operators::skew(Matrix<3, 1> vector) {
  float32_t a1 = vector.at(0, 0);
  float32_t a2 = vector.at(1, 0);
  float32_t a3 = vector.at(2, 0);

  return Matrix<3, 3>({
    0.0, -a3, a2,
    a3, 0.0, -a1,
    -a2, a1, 0.0
  });
}

Quaternion::Quaternion(float32_t quat[4]) {
  std::copy(quat, quat + 4, quat_);
}

Quaternion::Quaternion(const std::array<float32_t, 4> values) {
  std::copy(values.begin(), values.end(), quat_);
}

Quaternion::Quaternion(const Matrix<3, 1> vector, float32_t scalar) {
  quat_[0] = vector.at(0, 0);
  quat_[1] = vector.at(1, 0);
  quat_[2] = vector.at(2, 0);

  quat_[3] = scalar;
}

Quaternion::Quaternion(const Quaternion &other) {
  std::copy(other.quat_, other.quat_ + 4, quat_);
}

float32_t Quaternion::scalar() const {
  return quat_[3];
}

const Matrix<3, 1> Quaternion::vector() const {
  return Matrix<3, 1>(quat_);
}

// Implements the second algorithm in "Used methods"
// https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
Matrix<3, 1> Quaternion::apply(const Matrix<3, 1> &p) const {
  const Matrix<3, 1> v = vector();
  const float32_t w = scalar();

  return p + Matrix<3, 1>::cross(2 * v, Matrix<3, 1>::cross(v, p) + w * p);
}

Quaternion Quaternion::from_zyx(float32_t roll, float32_t pitch, float32_t yaw) {
  const float32_t cx = cos(roll/2.0);
  const float32_t cy = cos(pitch/2.0);
  const float32_t cz = cos(yaw/2.0);

  const float32_t sx = sin(roll/2.0);
  const float32_t sy = sin(pitch/2.0);
  const float32_t sz = sin(yaw/2.0);

  const float32_t w = cx * cy * cz + sx * sy * sz;
  const float32_t x = sx * cy * cz - cx * sy * sz;
  const float32_t y = cx * sy * cz + sx * cy * sz;
  const float32_t z = cx * cy * sz - sx * sy * cz;

  return Quaternion(Matrix<3, 1>({x, y, z}), w);
}

Quaternion Quaternion::compose(Quaternion a, Quaternion b) {
  const float32_t aw = a.scalar();
  const float32_t bw = b.scalar();
  
  auto a_v = a.vector();
  auto b_v = b.vector();

  auto vector = aw * b_v + bw * a_v + Matrix<3,1>::cross(a_v, b_v);

  return Quaternion(vector, aw * bw - a_v.transpose() * b_v);
}
