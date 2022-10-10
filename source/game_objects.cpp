#include "game_objects.h"
#include <d2d1helper.h>

game_shape::game_shape(const game_point* points, int pointCount)
{
  InitializeShape(points, pointCount, *this);
}

player_ship::player_ship() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
{
  static const game_point points[] = {
    game_point(0, -10),
    game_point(7, 10),
    game_point(-7, 10)
  };

  static const int pointCount = sizeof(points) / sizeof(game_point);

  outline = std::make_unique<game_shape>(points, pointCount);
}

bullet::bullet() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0), lifespanSeconds(1.0), outsideLevel(false)
{
}

std::unique_ptr<player_ship> CreatePlayerShip()
{
  return std::make_unique<player_ship>();
}

void InitializeShape(const game_point* points, int pointCount, game_shape& shape)
{
  for( int i = 0; i < pointCount; i++)
  {
    shape.points.push_back(points[i]);
    int endPointIndex = (i + 1) % pointCount;
    shape.lines.push_back(std::make_pair(points[i], points[endPointIndex]));
  }
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
  if( point.x >= line.first.x && point.x < line.second.x || point.x < line.first.x && point.x >= line.second.x )
  {
    float yIntercept = GetYIntercept(point.x, line);
    return yIntercept <= point.y;
  }
  return false;
}

float GetYIntercept(float x, const game_line& line)
{
  float cx = line.second.x - line.first.x;
  float cy = line.second.y - line.first.y;
  float m = cy / cx;
  float b = line.first.y - m * line.first.x;
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
