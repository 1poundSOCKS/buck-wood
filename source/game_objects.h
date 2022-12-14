#ifndef _game_object_
#define _game_object_

#include "game_level_data.h"

static const float defaultTargetSize = 40;

// void CreatePointsForPlayerThruster(std::back_insert_iterator<std::vector<game_point>> transformedPoints);
void CreatePointsForTarget(float size, std::back_insert_iterator<std::vector<game_point>> inserter);

consteval std::array<game_point, 3> GetPlayerGeometryData()
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

#endif
