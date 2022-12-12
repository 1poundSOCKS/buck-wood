#include "pch.h"
#include "level_object_collisions.h"

bool PlayerHasHitTheGround(
  const std::vector<game_point>& player, 
  const std::vector<game_line>::const_iterator groundBegin, 
  const std::vector<game_line>::const_iterator groundEnd)
{
  std::vector<int> interceptCounts;
  std::transform(
    player.cbegin(), 
    player.cend(), 
    std::back_inserter(interceptCounts),
    [groundBegin, groundEnd](auto& point)
    { return GetGroundInterceptCount(point, groundBegin, groundEnd); }
  );

  std::vector<bool> pointBelowBoundaryFlags;
  std::transform(
    interceptCounts.cbegin(),
    interceptCounts.cend(),
    std::back_inserter(pointBelowBoundaryFlags),
    [](auto& interceptCount) { return interceptCount % 2 == 1; }
  );

  return std::reduce(
    pointBelowBoundaryFlags.cbegin(),
    pointBelowBoundaryFlags.cend(),
    false,
    [](auto flag, auto below)
    { return flag || below; }
  );
}

bool BulletHasHitTheGround(
  const bullet& bullet, 
  const std::vector<game_line>::const_iterator groundBegin, 
  const std::vector<game_line>::const_iterator groundEnd)
{
  return GetGroundInterceptCount({ bullet.xPos, bullet.yPos }, groundBegin, groundEnd) % 2 ? true : false;
}

void GetBulletTargetCollisions(
  std::vector<bullet>::iterator bulletBegin, 
  std::vector<bullet>::iterator bulletEnd, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions)
{
  for( auto& bullet = bulletBegin; bullet != bulletEnd; ++bullet )
  {
    GetBulletTargetCollisions(*bullet, targetsBegin, targetsEnd, collisions);
  }  
}

void GetBulletTargetCollisions(
  bullet& bullet, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions)
{
  const game_point bulletPoint(bullet.xPos, bullet.yPos);

  for( auto& targetState = targetsBegin; targetState != targetsEnd; ++targetState )
  {
    if( PointInside(bulletPoint, targetState->shape) )
      collisions = bullet_target_collision { bullet, *targetState };
  }
}

int ProcessBulletTargetCollisions(
  std::vector<bullet_target_collision>::iterator collisionsBegin, 
  std::vector<bullet_target_collision>::iterator collisionsEnd)
{
  int activatedCount = 0;

  for( auto& collision = collisionsBegin; collision != collisionsEnd; ++collision )
  {
    collision->bullet.free = true;
    if( !collision->targetState.activated )
    {
      collision->targetState.activated = true;
      ++activatedCount;
    }
  }

  return activatedCount;
}

int GetGroundInterceptCount(const game_point& point, std::vector<game_line>::const_iterator groundBegin, std::vector<game_line>::const_iterator groundEnd)
{
  auto& groundFirst = *groundBegin;
  auto& groundLast = *std::prev(groundEnd);

  int count = GetLineInterceptCount(point, groundBegin, groundEnd);
  
  if( point.x >= groundFirst.start.x && point.y > groundFirst.start.y ) ++count;
  if( point.x <= groundLast.end.x && point.y > groundLast.end.y ) ++count;

  return count;
}
