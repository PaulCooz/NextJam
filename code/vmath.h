#pragma once

#include "raylib.h"
#include <algorithm>
#include <math.h>
#include <string>

static std::string Trim(std::string s) {
  int i = 0;
  while (i < s.size() && (s[i] == ' ' || s[i] == '\n'))
    i++;

  int j = s.size() - 1;
  while (0 <= j && (s[j] == ' ' || s[j] == '\n'))
    j--;
  return s.substr(i, j - i + 1);
}

static Color HexToRgb(std::string str) {
  Color color;

  color.r = std::stoi(str.substr(1, 2), 0, 16);
  color.g = std::stoi(str.substr(3, 2), 0, 16);
  color.b = std::stoi(str.substr(5, 2), 0, 16);

  if (str.length() == 9) {
    color.a = std::stoi(str.substr(7, 2), 0, 16);
  } else {
    color.a = 255;
  }

  return color;
}

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
