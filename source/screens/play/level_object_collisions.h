#ifndef _level_object_colllsions_
#define _level_object_collisions_

#include "level_objects.h"
#include "collisions.h"

[[nodiscard]] bool PlayerHasHitTheGround(
  const std::vector<game_point>& player, 
  const std::vector<game_line>::const_iterator groundBegin, 
  const std::vector<game_line>::const_iterator groundEnd) [[nothrow]];

bool BulletHasHitTheGround(
  const bullet& bullet, 
  const std::vector<game_line>::const_iterator groundBegin, 
  const std::vector<game_line>::const_iterator groundEnd);

bool BulletHasHitAnObject(
  const bullet& bullet, 
  const std::vector<game_line>::const_iterator linesBegin, 
  const std::vector<game_line>::const_iterator linesEnd);

void GetBulletTargetCollisions(
  std::vector<bullet>::iterator bulletBegin, 
  std::vector<bullet>::iterator bulletEnd, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions);

void GetBulletTargetCollisions(
  bullet& bullet, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions);

int GetGroundInterceptCount(float x, float y, auto groundBegin, auto groundEnd)
{
  auto& groundFirst = *groundBegin;
  auto& groundLast = *std::prev(groundEnd);

  int count = GetLineInterceptCount(game_point { x, y }, groundBegin, groundEnd);
  
  if( x >= groundFirst.start.x && y > groundFirst.start.y ) ++count;
  if( x <= groundLast.end.x && y > groundLast.end.y ) ++count;

  return count;
}

void GetGroundInterceptCounts(auto pointsBegin, auto pointsEnd, auto groundBegin, auto groundEnd, auto interceptCountsInserter)
{
  std::transform(
    pointsBegin, 
    pointsEnd, 
    interceptCountsInserter,
    [groundBegin, groundEnd](auto& point) { return GetGroundInterceptCount(point.x, point.y, groundBegin, groundEnd); }
  );
}

[[nodiscard]] bool GetBelowGroundFlag(float x, float y, auto groundBegin, auto groundEnd)
{
  return GetGroundInterceptCount(x, y, groundBegin, groundEnd) % 2 == 1;
}

void GetBelowGroundFlags(auto pointsBegin, auto pointsEnd, auto groundBegin, auto groundEnd, auto belowGroundFlagsInserter)
{
  std::vector<int> groundInterceptCounts;
  GetGroundInterceptCounts(pointsBegin, pointsEnd, groundBegin, groundEnd, std::back_inserter(groundInterceptCounts));

  std::transform(
    groundInterceptCounts.cbegin(),
    groundInterceptCounts.cend(),
    belowGroundFlagsInserter,
    [](auto& count) { return count % 2 == 1; }
  );
}

#endif
