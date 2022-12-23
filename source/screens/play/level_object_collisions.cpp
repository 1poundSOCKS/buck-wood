#include "pch.h"
#include "level_object_collisions.h"

[[nodiscard]] auto PlayerHasHitTheGround(
  const player_ship& player, 
  const level_ground_geometry& groundGeometry) -> bool [[nothrow]]
{
  return std::reduce(
    player.points.cbegin(),
    player.points.cend(),
    false,
    [&groundGeometry](auto hit, auto point)
    {
      return hit || CoordinateIsUnderground(point.x, point.y, groundGeometry);
    }
  );
}

void GetBulletTargetCollisions(
  std::vector<bullet>::iterator bulletBegin, 
  std::vector<bullet>::iterator bulletEnd, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions) [[nothrow]]
{
  for( auto& bullet = bulletBegin; bullet != bulletEnd; ++bullet )
  {
    if( !bullet->free ) GetBulletTargetCollisions(*bullet, targetsBegin, targetsEnd, collisions);
  }
}

void GetBulletTargetCollisions(
  bullet& bullet, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions) [[nothrow]]
{
  for( auto& targetState = targetsBegin; targetState != targetsEnd; ++targetState )
  {
    if( PointInsideObject(bullet.xPos, bullet.yPos, targetState->shape.cbegin(), targetState->shape.cend()) )
      collisions = bullet_target_collision { bullet, *targetState };
  }
}

[[nodiscard]] auto CoordinateIsUnderground(float x, float y, const level_ground_geometry& levelGoundGeometry) -> bool [[nothrow]]
{
  auto lineInterceptCount = GetLineInterceptCount({x, y}, levelGoundGeometry.lines.cbegin(), levelGoundGeometry.lines.cend());
  
  if( x >= levelGoundGeometry.groundStart.x && y > levelGoundGeometry.groundStart.y ) ++lineInterceptCount;
  if( x < levelGoundGeometry.groundEnd.x && y > levelGoundGeometry.groundEnd.y ) ++lineInterceptCount;

  return lineInterceptCount % 2 == 1;
}
