#ifndef LQR_H_
#define LQR_H_

#include "matrix.h"
#include "cstddef"

namespace controls {

template <size_t States, size_t Inputs>
class LQR {
  public:
    static constexpr int kNumTaylorExpansions = 5;

    LQR(Matrix<States, States> Q, Matrix<Inputs, Inputs> R):
      Q_(Q),
      R_(R) {}

    // Solves the DARE problem for an infinite horizon and outputs our K matrix
    // TODO(max): Is this the best way to do this? We are gonna run into numerical stability because of f32.
    Matrix<Inputs, States> SolveDare(Matrix<States, States> A, Matrix<States, Inputs> B, float32_t dt, int steps=500) {
      Matrix<States, States> A_d;
      Matrix<States, Inputs> B_d;

      DiscretizeAB(&A_d, &B_d, A, B, dt);

      Matrix<States, States> Q_d = Q_ * dt;
      Matrix<Inputs, Inputs> R_d = R_ * (1.0 / dt);

      Matrix<States, States> X_k = Q_d;

      auto G = B_d * R_d.inverse() * B_d.transpose();

      for (int i = 0; i < steps; i++) {
        X_k = A_d.transpose() * X_k * (Matrix<States, States>::Identity() + G * X_k).inverse() * A_d + Q_d;
      }

      auto K = (R_d + B_d.transpose() * X_k * B_d).inverse() * B_d.transpose() * X_k * A_d;

      return K;
    }
  private:
    // We are discretizing by doing two seperate taylor expansions for A_d and B_d 
    void DiscretizeAB(Matrix<States, States> &A_d, Matrix<States, Inputs> &B_d, Matrix<States, States> A, Matrix<States, States> B, float32_t dt) {
      // A_d = e^A(dt)
      auto term1 = A * dt;
      A_d = Matrix<States, States>::Identity() + term1;

      // Taylor expand a set number of times
      for (size_t i = 2; i <= kNumTaylorExpansions; i++) {
        term1 = (term1 * A * dt) / i;
        A_d += term1;
      }

      // B_d = (int(0, dt, e^At dt) * B)
      
      auto term2 = (A * dt * dt) / 2.0f;
      auto inner_accum = term2;
      for (size_t i = 2; i <= kNumTaylorExpansions; i++) {
        term2 = (term2 * A * dt) / (i+1);
        inner_accum += term2;
      }

      B_d = (Matrix<States, States>::Identity() * dt + inner_accum) * B;
    }
    
    Matrix<States, States> Q_;
    Matrix<Inputs, Inputs> R_;
};

};

#endif // LQR_H_
