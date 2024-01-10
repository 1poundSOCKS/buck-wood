#pragma once

#include "game_angle.h"

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

struct game_point
{
  float x = 0;
  float y = 0;

  [[nodiscard]] auto DistanceTo(game_point p) const -> float;
  [[nodiscard]] auto AngleTo(game_point p) const -> float;
};

[[nodiscard]] inline auto game_point::DistanceTo(game_point p) const -> float
{
  return GetDistanceBetweenPoints(x, y, p.x, p.y);
}

[[nodiscard]] inline auto game_point::AngleTo(game_point p) const -> float
{
  return CalculateAngle(x, y, p.x, p.y);
}
