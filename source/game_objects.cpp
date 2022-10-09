#include "game_objects.h"
#include <d2d1helper.h>

game_shape::game_shape(const game_point* points, int pointCount)
{
  InitializeShape(points, pointCount, *this);
}

game_object::game_object() : game_object(NULL, 0)
{
}

game_object::game_object(const game_point* points, int pointCount) : size(20.0), xPos(0), yPos(0), forceX(0), forceY(0), xVelocity(0), yVelocity(0), angle(0), spin(0)
{
  outline = std::make_unique<game_shape>(points, pointCount);
  InitializeShape(points, pointCount, *outline);
}

std::unique_ptr<game_object> CreatePlayerObject()
{
  const game_point points[] = {
    game_point(0, -10),
    game_point(7, 10),
    game_point(-7, 10)
  };

  const int pointCount = sizeof(points) / sizeof(game_point);

  return std::make_unique<game_object>(points, pointCount);
}

std::unique_ptr<game_object> CreateCursorObject()
{
  return std::make_unique<game_object>();
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

void CalculateTransformedPoints(const game_object& gameObject, std::list<game_point>& transformedPoints)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(gameObject.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(gameObject.xPos, gameObject.yPos);
  const D2D1::Matrix3x2F matrix = rotate * translate;

  for( auto point: gameObject.outline->points )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = point.x;
    inPoint.y = point.y;
    D2D1_POINT_2F outPoint = matrix.TransformPoint(inPoint);
    transformedPoints.push_back(game_point(outPoint.x, outPoint.y));
  }
}
