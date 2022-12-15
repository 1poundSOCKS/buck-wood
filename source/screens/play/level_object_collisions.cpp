#include "pch.h"
#include "level_object_collisions.h"

bool PlayerHasHitTheGround(
  const std::vector<game_point>& player, 
  const std::vector<game_line>::const_iterator groundBegin, 
  const std::vector<game_line>::const_iterator groundEnd)
{
  std::vector<bool> belowGroundFlags;
  GetBelowGroundFlags(player.cbegin(), player.cend(), groundBegin, groundEnd, std::back_inserter(belowGroundFlags));

  return std::reduce(
    belowGroundFlags.cbegin(),
    belowGroundFlags.cend(),
    false,
    [](auto flag, auto below) { return flag || below; }
  );
}

bool BulletHasHitTheGround(
  const bullet& bullet, 
  const std::vector<game_line>::const_iterator groundBegin, 
  const std::vector<game_line>::const_iterator groundEnd)
{
  return GetBelowGroundFlag(bullet.xPos, bullet.yPos, groundBegin, groundEnd);
}

bool BulletHasHitAnObject(
  const bullet& bullet, 
  const std::vector<game_line>::const_iterator linesBegin, 
  const std::vector<game_line>::const_iterator linesEnd)
{
  return GetPointInsideShapeFlag(bullet.xPos, bullet.yPos, linesBegin, linesEnd);
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
    if( !bullet->free ) GetBulletTargetCollisions(*bullet, targetsBegin, targetsEnd, collisions);
  }
}

void GetBulletTargetCollisions(
  bullet& bullet, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions)
{
  for( auto& targetState = targetsBegin; targetState != targetsEnd; ++targetState )
  {
    if( GetPointInsideShapeFlag(bullet.xPos, bullet.yPos, targetState->shape.cbegin(), targetState->shape.cend()) )
      collisions = bullet_target_collision { bullet, *targetState };
  }
}
