#ifndef MATRIX_H_
#define MATRIX_H_

#include "arm_math.h"
#include <Arduino.h>

// Wrapper around a f32 cmsis dsp matrix object
// TODO(max): Chain optimization is required

template <int R, int C> class Matrix;
template <int Rows, int Cols> class Matrix {
public:
  static constexpr size_t kDataSize = Rows * Cols;

  template <int R, int C> friend class Matrix;

  Matrix(const float32_t data[kDataSize])
      : matrix_instance_{Rows, Cols, matrix_data_} {
    std::copy(data, data + kDataSize, matrix_data_);
  }
  
  explicit Matrix(const std::array<float32_t, kDataSize> values)
      : matrix_instance_{Rows, Cols, matrix_data_} {
    std::copy(values.begin(), values.end(), matrix_data_);
  }

  Matrix(const Matrix &other) : matrix_instance_{Rows, Cols, matrix_data_} {
    std::copy(other.matrix_data_, other.matrix_data_ + kDataSize, matrix_data_);
  }

  Matrix &operator=(const Matrix &other) {
    std::copy(other.matrix_data_, other.matrix_data_ + kDataSize, matrix_data_);

    matrix_instance_ = {Rows, Cols, matrix_data_};
    return *this;
  }

  static constexpr Matrix Zero() {
    Matrix matrix{};
    std::fill(matrix.matrix_data_, matrix.matrix_data_ + kDataSize, 0.0f);
    matrix.matrix_instance_ = {Rows, Cols, matrix.matrix_data_};
    return matrix;
  }

  static constexpr Matrix Identity() {
    static_assert(Rows == Cols, "Identity matrix must be square");

    Matrix matrix = Zero();

    for (int i = 0; i < Rows; i++) {
      matrix.set(i, i, 1.0f);
    }

    return matrix;
  }

  Matrix operator+(const Matrix<Rows, Cols> &rhs) const {
    Matrix result;

    arm_mat_add_f32(&matrix_instance_, &rhs.matrix_instance_,
                    &result.matrix_instance_);

    return result;
  }

  Matrix operator-(const Matrix<Rows, Cols> &rhs) const {
    Matrix result;

    arm_mat_sub_f32(&matrix_instance_, &rhs.matrix_instance_,
                    &result.matrix_instance_);

    return result;
  }

  template <int RhsCols>
  Matrix<Rows, RhsCols> operator*(const Matrix<Cols, RhsCols> &rhs) const {
    Matrix<Rows, RhsCols> result;

    arm_mat_mult_f32(&matrix_instance_, &rhs.matrix_instance_,
                     &result.matrix_instance_);

    return result;
  }

  Matrix<Rows, Cols> operator*(const float32_t scalar) const {
    Matrix<Rows, Cols> out;

    arm_mat_scale_f32(&matrix_instance_, scalar, &out.matrix_instance_);

    return out;
  }

  friend Matrix operator*(const float32_t rhs, const Matrix &lhs) {
    return lhs * rhs;
  }

  Matrix<Rows, Cols> operator/(const float32_t scalar) const {
    Matrix<Rows, Cols> out;

    arm_mat_scale_f32(&matrix_instance_, 1.0f / scalar, &out.matrix_instance_);

    return out;
  }

  Matrix<Cols, Rows> transpose() const {
    Matrix<Cols, Rows> result;

    arm_mat_trans_f32(&matrix_instance_, &result.matrix_instance_);

    return result;
  }

  Matrix inverse() const {
    Matrix result;

    arm_mat_inverse_f32(&matrix_instance_, &result.matrix_instance_);

    return result;
  }

  float32_t norm() const {
    static_assert(Cols == 1, "Euclidian norm only defined for vectors");

    float32_t result = 0;
    // We const cast because the version of cmsis we're using doesn't have a const here.
    arm_rms_f32(const_cast<float32_t *>(matrix_data_), Rows, &result);
    return result;
  }
  
  // Matrix cross product which is only valid for 3, 1 matrices.
  static Matrix<3, 1> cross(Matrix<3, 1> a, Matrix<3, 1> b) {
    const float32_t a1 = a.at(0, 0);
    const float32_t a2 = a.at(1, 0);
    const float32_t a3 = a.at(2, 0);

    const float32_t b1 = b.at(0, 0);
    const float32_t b2 = b.at(1, 0);
    const float32_t b3 = b.at(2, 0);

    return Matrix<3, 1>({a2 * b3 - a3 * b2, a3 * b1 - a1 * b3, a1 * b2 - a2 * b1});
  }

  constexpr inline void set(int i, int j, float32_t data) {
    matrix_data_[i * Cols + j] = data;
  }

  constexpr inline const float32_t at(int i, int j) const {
    return matrix_data_[i * Cols + j];
  }

  void print() const {
    for (int i = 0; i < Rows; i++) {
      for (int j = 0; j < Cols; j++) {
        Serial.print(at(i, j));
        if (j != (Cols - 1)) {
          Serial.print(" ,");
        }
      }
      Serial.println();
    }
  }

protected:
  Matrix() : matrix_instance_{Rows, Cols, matrix_data_} {}
  arm_matrix_instance_f32 matrix_instance_;

  // We want to store the data here so it doesnt go out scope
  float32_t matrix_data_[kDataSize];
};

#endif // MATRIX_H_
