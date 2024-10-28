#pragma once

#include "raylib.h"
#include <algorithm>
#include <math.h>

float Length(Vector2 vec) { return sqrt(vec.x * vec.x + vec.y * vec.y); }

Vector2 Mult(const Vector2& vec, float scale) {
  Vector2 res = vec;
  res.x *= scale;
  res.y *= scale;
  return res;
}

Vector2 Normalize(const Vector2& vec) {
  float len = Length(vec);
  if (len == 0.0)
    len = 1.0;

  return Mult(vec, 1.0 / len);
}
