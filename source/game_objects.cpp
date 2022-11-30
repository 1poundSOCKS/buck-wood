#include "pch.h"
#include "game_objects.h"
#include "framework.h"

game_line::game_line(const game_point& start, const game_point& end) : start(start), end(end)
{
}

game_line::game_line(float startX, float startY, float endX, float endY) : start(startX, startY), end(endX, endY)
{
}

void CreatePointsForTarget(float size, std::back_insert_iterator<std::vector<game_point>> inserter)
{
  float halfSize = size / 2;
  inserter = game_point(0, - halfSize);
  inserter = game_point(halfSize, 0);
  inserter = game_point(0, halfSize);
  inserter = game_point(- halfSize, 0);
}

void CreatePointsForPlayer(std::back_insert_iterator<std::vector<game_point>> points)
{
  points = game_point(0, -10);
  points = game_point(7, 10);
  points = game_point(-7, 10);
}

void CreatePointsForPlayerThruster(std::back_insert_iterator<std::vector<game_point>> points)
{
  points = game_point(5, 14);
  points = game_point(-5, 14);
}
