#include "pch.h"
#include "game_math.h"

int CalculateAngle(float x1, float y1, float x2, float y2)
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.5;
}

bool PointsInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area)
{
  int matchingPointCount = 0;
  for( auto point = begin; point != end; ++point )
  {
    if( PointInside(*point, area) ) return true;
  }

  return false;
}

bool PointInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area)
{
  int matchingPointCount = 0;
  for( auto point = begin; point != end; ++point )
  {
    if( PointInside(*point, area) ) return true;
  }

  return false;
}

bool PointInside(const game_point& point, const std::vector<game_line>& area)
{
  int matchingLines = 0;
  for( const auto& line: area )
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

float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
  float cx = x2 - x1;
  float cy = y2 - y1;
  return sqrt( cx * cx + cy * cy );
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
