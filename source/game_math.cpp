#include "game_math.h"
#include <d2d1.h>

int CalculateAngle(float x1, float y1, float x2, float y2)
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.5;
}

bool PointsInside(const std::list<game_point>& points, const game_shape& shape)
{
  int matchingPointCount = 0;
  for( const auto& point: points )
  {
    if( PointInside(point, shape) ) matchingPointCount++;
  }

  return matchingPointCount == points.size();
}

bool PointInside(const std::list<game_point>& points, const game_shape& shape)
{
  int matchingPointCount = 0;
  for( const auto& point: points )
  {
    if( PointInside(point, shape) ) return true;
  }

  return false;
}

bool PointInside(const game_point& point, const game_shape& shape)
{
  int matchingLines = 0;
  for( const auto& line: shape.lines )
  {
    if( AddLineToInterceptCount(line, point) ) matchingLines++;
  }
  
  return ( matchingLines % 2 > 0 );
}

bool AddLineToInterceptCount(const game_line& line, const game_point& point)
{
  if( point.x >= line.start.x && point.x < line.end.x || point.x < line.start.x && point.x >= line.end.x )
  {
    float yIntercept = GetYIntercept(point.x, line);
    return yIntercept <= point.y;
  }
  return false;
}

float GetYIntercept(float x, const game_line& line)
{
  float cx = line.end.x - line.start.x;
  float cy = line.end.y - line.start.y;
  float m = cy / cx;
  float b = line.start.y - m * line.start.x;
  return m * x + b;
}

void TransformPlayerShip(const player_ship& player, std::list<game_point>& transformedPoints)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(player.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(player.xPos, player.yPos);
  const D2D1::Matrix3x2F matrix = rotate * translate;

  for( auto point: player.outline->points )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = point.x;
    inPoint.y = point.y;
    D2D1_POINT_2F outPoint = matrix.TransformPoint(inPoint);
    transformedPoints.push_back(game_point(outPoint.x, outPoint.y));
  }
}
