#include <Arduino.h>
#include <Arduino.h>

const float DT = 0.2f;

const float TRUE_PX0 = 0.0f;
const float TRUE_PY0 = 0.0f;
const float TRUE_PZ0 = 0.0f;

const float TRUE_VX  = 2.0f;
const float TRUE_VY  = 1.0f;
const float TRUE_VZ  = 0.5f;

const float NOISE_PX = 2.0f;
const float NOISE_PY = 2.0f;
const float NOISE_PZ = 3.0f;

const float Q_POS = 0.01f;
const float Q_VEL = 0.1f;

const float R_PX = 4.0f;
const float R_PY = 4.0f;
const float R_PZ = 9.0f;

struct State6 {
  float px, py, pz;
  float vx, vy, vz;
};

class Kalman6D {
public:
  Kalman6D() {
    x.px = 0.0f; x.py = 0.0f; x.pz = 0.0f;
    x.vx = 0.0f; x.vy = 0.0f; x.vz = 0.0f;

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        P[i][j] = (i == j) ? 1000.0f : 0.0f;

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        Q[i][j] = 0.0f;

    Q[0][0] = Q_POS;
    Q[1][1] = Q_POS;
    Q[2][2] = Q_POS;
    Q[3][3] = Q_VEL;
    Q[4][4] = Q_VEL;
    Q[5][5] = Q_VEL;

    R[0][0] = R_PX; R[0][1] = 0.0f; R[0][2] = 0.0f;
    R[1][0] = 0.0f; R[1][1] = R_PY; R[1][2] = 0.0f;
    R[2][0] = 0.0f; R[2][1] = 0.0f; R[2][2] = R_PZ;

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        I[i][j] = (i == j) ? 1.0f : 0.0f;

    setA(DT);
  }

  void setA(float dt) {
    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        A[i][j] = 0.0f;

    A[0][0] = 1.0f; A[0][3] = dt;
    A[1][1] = 1.0f; A[1][4] = dt;
    A[2][2] = 1.0f; A[2][5] = dt;
    A[3][3] = 1.0f;
    A[4][4] = 1.0f;
    A[5][5] = 1.0f;
  }

  void predict() {
    State6 xp;
    xp.px = x.px + x.vx * DT;
    xp.py = x.py + x.vy * DT;
    xp.pz = x.pz + x.vz * DT;
    xp.vx = x.vx;
    xp.vy = x.vy;
    xp.vz = x.vz;
    x = xp;

    float AP[6][6];
    float APAT[6][6];

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j) {
        float sum = 0.0f;
        for (int k = 0; k < 6; ++k)
          sum += A[i][k] * P[k][j];
        AP[i][j] = sum;
      }

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j) {
        float sum = 0.0f;
        for (int k = 0; k < 6; ++k)
          sum += AP[i][k] * A[j][k];
        APAT[i][j] = sum;
      }

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        P[i][j] = APAT[i][j] + Q[i][j];
  }

  void update(float meas_px, float meas_py, float meas_pz) {
    float y[3];
    y[0] = meas_px - x.px;
    y[1] = meas_py - x.py;
    y[2] = meas_pz - x.pz;

    float S[3][3];
    S[0][0] = P[0][0] + R[0][0];
    S[0][1] = P[0][1] + R[0][1];
    S[0][2] = P[0][2] + R[0][2];
    S[1][0] = P[1][0] + R[1][0];
    S[1][1] = P[1][1] + R[1][1];
    S[1][2] = P[1][2] + R[1][2];
    S[2][0] = P[2][0] + R[2][0];
    S[2][1] = P[2][1] + R[2][1];
    S[2][2] = P[2][2] + R[2][2];

    float S_inv[3][3];
    if (!invert3x3(S, S_inv)) return;

    float PHt[6][3];
    for (int i = 0; i < 6; ++i) {
      PHt[i][0] = P[i][0];
      PHt[i][1] = P[i][1];
      PHt[i][2] = P[i][2];
    }

    float K[6][3];
    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 3; ++j) {
        float sum = 0.0f;
        for (int k = 0; k < 3; ++k)
          sum += PHt[i][k] * S_inv[k][j];
        K[i][j] = sum;
      }

    float dx[6];
    for (int i = 0; i < 6; ++i) {
      float sum = 0.0f;
      for (int j = 0; j < 3; ++j)
        sum += K[i][j] * y[j];
      dx[i] = sum;
    }

    x.px += dx[0];
    x.py += dx[1];
    x.pz += dx[2];
    x.vx += dx[3];
    x.vy += dx[4];
    x.vz += dx[5];

    float KH[6][6];
    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        KH[i][j] = (j < 3) ? K[i][j] : 0.0f;

    float IminusKH[6][6];
    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        IminusKH[i][j] = I[i][j] - KH[i][j];

    float newP[6][6];
    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j) {
        float sum = 0.0f;
        for (int k = 0; k < 6; ++k)
          sum += IminusKH[i][k] * P[k][j];
        newP[i][j] = sum;
      }

    for (int i = 0; i < 6; ++i)
      for (int j = 0; j < 6; ++j)
        P[i][j] = newP[i][j];
  }

  State6 getState() const { return x; }

private:
  State6 x;
  float P[6][6];
  float Q[6][6];
  float R[3][3];
  float A[6][6];
  float I[6][6];

  bool invert3x3(float M[3][3], float Minv[3][3]) {
    float det =
      M[0][0]*(M[1][1]*M[2][2] - M[1][2]*M[2][1]) -
      M[0][1]*(M[1][0]*M[2][2] - M[1][2]*M[2][0]) +
      M[0][2]*(M[1][0]*M[2][1] - M[1][1]*M[2][0]);

    if (fabs(det) < 1e-6f) return false;

    float invDet = 1.0f / det;

    Minv[0][0] =  (M[1][1]*M[2][2] - M[1][2]*M[2][1]) * invDet;
    Minv[0][1] = -(M[0][1]*M[2][2] - M[0][2]*M[2][1]) * invDet;
    Minv[0][2] =  (M[0][1]*M[1][2] - M[0][2]*M[1][1]) * invDet;

    Minv[1][0] = -(M[1][0]*M[2][2] - M[1][2]*M[2][0]) * invDet;
    Minv[1][1] =  (M[0][0]*M[2][2] - M[0][2]*M[2][0]) * invDet;
    Minv[1][2] = -(M[0][0]*M[1][2] - M[0][2]*M[1][0]) * invDet;

    Minv[2][0] =  (M[1][0]*M[2][1] - M[1][1]*M[2][0]) * invDet;
    Minv[2][1] = -(M[0][0]*M[2][1] - M[0][1]*M[2][0]) * invDet;
    Minv[2][2] =  (M[0][0]*M[1][1] - M[0][1]*M[1][0]) * invDet;

    return true;
  }
};

Kalman6D kf;
State6 trueState;
unsigned long lastUpdateMs = 0;

float randMinusOneToOne() {
  long r = random(0L, 10000L);
  return (r / 5000.0f) - 1.0f;
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));

  trueState.px = TRUE_PX0;
  trueState.py = TRUE_PY0;
  trueState.pz = TRUE_PZ0;
  trueState.vx = TRUE_VX;
  trueState.vy = TRUE_VY;
  trueState.vz = TRUE_VZ;

  lastUpdateMs = millis();

  Serial.println(F("t,true_px,true_py,true_pz,meas_px,meas_py,meas_pz,est_px,est_py,est_pz,est_vx,est_vy,est_vz"));
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs < (unsigned long)(DT * 1000.0f)) return;
  lastUpdateMs += (unsigned long)(DT * 1000.0f);

  static float t = 0.0f;
  t += DT;

  trueState.px += trueState.vx * DT;
  trueState.py += trueState.vy * DT;
  trueState.pz += trueState.vz * DT;

  float meas_px = trueState.px + randMinusOneToOne() * NOISE_PX;
  float meas_py = trueState.py + randMinusOneToOne() * NOISE_PY;
  float meas_pz = trueState.pz + randMinusOneToOne() * NOISE_PZ;

  kf.predict();
  kf.update(meas_px, meas_py, meas_pz);

  State6 est = kf.getState();

  Serial.print(t, 2); Serial.print(",");
  Serial.print(trueState.px); Serial.print(",");
  Serial.print(trueState.py); Serial.print(",");
  Serial.print(trueState.pz); Serial.print(",");
  Serial.print(meas_px); Serial.print(",");
  Serial.print(meas_py); Serial.print(",");
  Serial.print(meas_pz); Serial.print(",");
  Serial.print(est.px); Serial.print(",");
  Serial.print(est.py); Serial.print(",");
  Serial.print(est.pz); Serial.print(",");
  Serial.print(est.vx); Serial.print(",");
  Serial.print(est.vy); Serial.print(",");
  Serial.println(est.vz);
}
