#pragma once

#include "geometry.h"

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
