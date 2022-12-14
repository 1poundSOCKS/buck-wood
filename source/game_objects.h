#ifndef _game_object_
#define _game_object_

#include "game_level_data.h"

static const float defaultTargetSize = 40;

constexpr std::array<game_point, 3> GetPlayerGeometryData()
{
  return {
    game_point { 0, -10 },
    game_point { 7, 10 },
    game_point { -7, 10 }
  };
}

consteval std::array<game_point, 2> GetPlayerThrusterGeometryData()
{
  return {
    game_point { 5, 14 },
    game_point { -5, 14 }
  };
}

constexpr std::array<game_point, 4> GetTargetGeometryData(float size)
{
  float halfSize = size / 2;
  return {
    game_point { 0, -halfSize },
    game_point { halfSize, 0 },
    game_point { 0, halfSize },
    game_point { -halfSize, 0 }
  };
}

consteval std::array<game_point, 4> GetDefaultTargetGeometryData()
{
  return GetTargetGeometryData(40);
}

#endif
