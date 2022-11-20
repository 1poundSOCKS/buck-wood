#include "pch.h"
#include "game_objects.h"
#include "framework.h"

game_line::game_line(const game_point& start, const game_point& end) : start(start), end(end)
{
}

game_line::game_line(float startX, float startY, float endX, float endY) : start(startX, startY), end(endX, endY)
{
}

void CreatePointsForTarget(float x, float y, float size, std::back_insert_iterator<std::vector<game_point>> inserter)
{
  float halfSize = size / 2;
  inserter = game_point(x, y - halfSize);
  inserter = game_point(x + halfSize, y);
  inserter = game_point(x, y + halfSize);
  inserter = game_point(x - halfSize, y);
}

void CreatePointsForPlayer(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> transformedPoints)
{
  std::vector<game_point> points;
  points.reserve(3);
  points.push_back(game_point(0, -10));
  points.push_back(game_point(7, 10));
  points.push_back(game_point(-7, 10));
  TransformPoints(points.begin(), points.end(), transformedPoints, angle, x, y);
}

void CreatePointsForPlayerThruster(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> transformedPoints)
{
  std::vector<game_point> points;
  points.reserve(2);
  points.push_back(game_point(5, 14));
  points.push_back(game_point(-5, 14));
  TransformPoints(points.begin(), points.end(), transformedPoints, angle, x, y);
}

void TransformPoints(std::vector<game_point>::iterator begin, std::vector<game_point>::iterator end, std::back_insert_iterator<std::vector<game_point>> transformedPoints, float angle, float x, float y)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(x, y);
  const D2D1::Matrix3x2F matrix = rotate * translate;

  std::transform(begin, end, transformedPoints, [matrix](const auto& point){
    D2D1_POINT_2F inPoint(point.x, point.y);
    D2D1_POINT_2F outPoint = matrix.TransformPoint(inPoint);
    return game_point(outPoint.x, outPoint.y);
  });
}
