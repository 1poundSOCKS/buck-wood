#pragma once

#define PI 3.14159265f
#define DEGTORAD(D)((D * PI) / 180.0f)
#define RADTODEG(R)((180.0f * R) / PI)

[[nodiscard]] inline auto CalculateVectorX(float value, float angle) -> float
{
  return value * sin(DEGTORAD(angle));
}

[[nodiscard]] inline auto CalculateVectorY(float value, float angle) -> float
{
  return value * cos(DEGTORAD(angle));
}

inline auto CalculateAngle(float x1, float y1, float x2, float y2) -> float
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.0f;
}

inline float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
  float cx = x2 - x1;
  float cy = y2 - y1;
  return sqrt( cx * cx + cy * cy );
}

#include "game_point.h"
#include "game_angle.h"
#include "game_velocity.h"
