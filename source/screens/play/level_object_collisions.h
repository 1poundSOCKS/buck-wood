#ifndef _level_object_colllsions_
#define _level_object_collisions_

#include "level_geometry.h"
#include "level_objects.h"
#include "collisions.h"

struct bullet_target_collision
{
  bullet& bullet;
  target_state& targetState;
};

[[nodiscard]] auto PlayerHasHitTheGround(
  const player_ship& player, 
  const level_ground_geometry& groundGeometry) -> bool [[nothrow]];

void GetBulletTargetCollisions(
  std::vector<bullet>::iterator bulletBegin, 
  std::vector<bullet>::iterator bulletEnd, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions) [[nothrow]];

void GetBulletTargetCollisions(
  bullet& bullet, 
  std::vector<target_state>::iterator targetsBegin, 
  std::vector<target_state>::iterator targetsEnd,
  std::back_insert_iterator<std::vector<bullet_target_collision>> collisions) [[nothrow]];

[[nodiscard]] auto CoordinateIsUnderground(float x, float y, const level_ground_geometry& levelGroundGeometry) -> bool [[nothrow]];

#endif
