#include "game_objects.h"
#include <fstream>
#include <numeric>
#include <format>
#include "framework/data_files.h"

game_line::game_line(const game_point& start, const game_point& end) : start(start), end(end)
{
}

game_line::game_line(float startX, float startY, float endX, float endY) : start(startX, startY), end(endX, endY)
{
}

game_shape::game_shape()
{
}

game_shape::game_shape(const game_point* points, int pointCount)
{
  InitializeShape(points, pointCount, *this);
}

game_shape::game_shape(const std::vector<game_point>& pointsToCopy)
{
  std::copy( pointsToCopy.begin(), pointsToCopy.end(), std::back_inserter(points) );
  CreateShapeLinesFromPoints(lines, points);
}

game_shape::game_shape(const game_level_object_data& objectData)
{
  std::copy(objectData.points.cbegin(), objectData.points.end(), std::back_inserter(points));
  CreateShapeLinesFromPoints(lines, points);
}

mouse_cursor::mouse_cursor()
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  lines.push_back(game_line(0,-cursorSize,0,-cursorSizeGap));
  lines.push_back(game_line(0,cursorSize,0,cursorSizeGap));
  lines.push_back(game_line(-cursorSize,0,-cursorSizeGap,0));
  lines.push_back(game_line(cursorSize,0,cursorSizeGap,0));
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
  thruster = std::make_unique<game_line>(game_point(5, 14), game_point(-5, 14));
}

bullet::bullet(float x, float y, float range) : startX(x), startY(y), xPos(x), yPos(y), range(range), xVelocity(0), yVelocity(0), angle(0), outsideLevel(false)
{
}

target::target(float x, float y, float size) : state(DEACTIVATED)
{
  InitializeTargetShape(x, y, size, shape);
}

game_level::game_level(const game_level_data& gameLevelData)
: name(gameLevelData.name),
  playerStartPosX(gameLevelData.playerStartPosX),
  playerStartPosY(gameLevelData.playerStartPosY),
  timeLimitInSeconds(gameLevelData.timeLimitInSeconds)
{
  boundary = std::make_unique<game_shape>(gameLevelData.boundaryPoints);

  std::unique_ptr<game_level_object_data> objectData = std::make_unique<game_level_object_data>();
  for( const auto& object: gameLevelData.objects )
  {
    std::unique_ptr<game_shape> gameShape = std::make_unique<game_shape>(object);
    objects.push_back(std::move(gameShape));
  }
  
  for( const auto& t: gameLevelData.targets )
  {
    auto levelTarget = std::make_unique<target>(t.x, t.y, 40.0f);
    targets.push_back(std::move(levelTarget));
  }
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
    shape.lines.push_back(game_line(points[i].x, points[i].y, points[endPointIndex].x, points[endPointIndex].y));
  }
}

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

void InitializeTargetShape(float x, float y, float size, game_shape& shape)
{
  float halfSize = size / 2;

  const game_point points[] = {
    game_point(x, y - halfSize ),
    game_point(x + halfSize, y),
    game_point(x, y + halfSize),
    game_point(x - halfSize, y)
  };

  static const int pointCount = sizeof(points) / sizeof(game_point);

  InitializeShape(points, pointCount, shape);
}
