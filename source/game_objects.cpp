#include "game_objects.h"
#include <fstream>
#include <numeric>
#include <format>
#include "framework/data_files.h"
#include "game_math.h"

game_line::game_line(const game_point& start, const game_point& end) : start(start), end(end)
{
}

game_line::game_line(float startX, float startY, float endX, float endY) : start(startX, startY), end(endX, endY)
{
}

// game_shape::game_shape()
// {
// }

// game_shape::game_shape(const game_point* points, int pointCount)
// {
//   InitializeShape(points, pointCount, *this);
// }

// game_shape::game_shape(const std::vector<game_point>& pointsToCopy)
// {
//   std::copy( pointsToCopy.begin(), pointsToCopy.end(), std::back_inserter(points) );
//   CreateShapeLinesFromPoints(lines, points);
// }

// game_shape::game_shape(const game_level_object_data& objectData)
// {
//   std::copy(objectData.points.cbegin(), objectData.points.end(), std::back_inserter(points));
//   CreateShapeLinesFromPoints(lines, points);
// }

player_ship::player_ship() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
{
}

bullet::bullet(float x, float y, float range) : startX(x), startY(y), xPos(x), yPos(y), range(range), xVelocity(0), yVelocity(0), angle(0), outsideLevel(false)
{
}

// void InitializeShape(const game_point* points, int pointCount, game_shape& shape)
// {
//   for( int i = 0; i < pointCount; i++)
//   {
//     shape.points.push_back(points[i]);
//     int endPointIndex = (i + 1) % pointCount;
//     shape.lines.push_back(game_line(points[i].x, points[i].y, points[endPointIndex].x, points[endPointIndex].y));
//   }
// }

void CreateShapeLinesFromPoints(std::list<game_line>& lines, const std::list<game_point>& points)
{
  std::list<game_point>::const_iterator i = points.begin();
  
  while( i != points.end()  )
  {
    const game_point& point1 = *i;
    i++;
    if( i != points.end() )
    {
      const game_point& point2 = *i;
      lines.push_back(game_line(point1.x, point1.y, point2.x, point2.y));
    }
    else
    {
      const game_point& point2 = *points.begin();
      lines.push_back(game_line(point1.x, point1.y, point2.x, point2.y));
    }
  }
}

// void InitializeTargetShape(float x, float y, float size, game_shape& shape)
// {
//   float halfSize = size / 2;

//   const game_point points[] = {
//     game_point(x, y - halfSize ),
//     game_point(x + halfSize, y),
//     game_point(x, y + halfSize),
//     game_point(x - halfSize, y)
//   };

//   static const int pointCount = sizeof(points) / sizeof(game_point);

//   InitializeShape(points, pointCount, shape);
// }

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
