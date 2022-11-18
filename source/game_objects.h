#ifndef _game_object_
#define _game_object_

#include <memory>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include "game_level_data.h"

struct game_line
{
  game_line(const game_point& start, const game_point& end);
  game_line(float startX, float startY, float endX, float endY);

  game_point start, end;
};

struct player_ship
{
  player_ship();

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  bool thrusterOn = false;
};

struct bullet
{
  bullet(float x, float y, float range);

  float startX, startY;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float range;
  bool outsideLevel;
};

void CreateShapeLinesFromPoints(std::list<game_line>& lines, const std::list<game_point>& points);

void CreatePointsForPlayer(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> inserter);
void CreatePointsForPlayerThruster(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> transformedPoints);
void CreatePointsForTarget(float x, float y, float size, std::back_insert_iterator<std::vector<game_point>> inserter);

template <typename T>
void CreateConnectedLines(typename std::vector<T>::const_iterator begin, 
                          typename std::vector<T>::const_iterator end, 
                          std::back_insert_iterator<std::vector<game_line>> insertIterator,
                          float x=0, float y=0)
{
  std::transform(std::next(begin), end, begin, insertIterator, [x, y](const auto& point2, const auto& point1)
  {
    game_point start(point1.x + x, point1.y + y);
    game_point end(point2.x + x, point2.y + y);
    return game_line(start, end);
  });

  typename std::vector<T>::const_iterator last = std::prev(end);

  game_point startPoint(last->x + x, last->y + y);
  game_point endPoint(begin->x + x, begin->y + y);
  insertIterator = game_line(startPoint, endPoint);
};

#endif
