#include <Arduino.h>

#include "arm_math.h"
#include "matrix.h"
#include "lqr.h"

#include <memory>


void setup() {
  Serial.begin(115200);

  Matrix<1,2> mat({1, 2});

  while (!Serial) {
  }
}

void loop() {}
