#ifndef _level_object_colllsions_
#define _level_object_collisions_

#include "level_objects.h"

bool PlayerHasHitTheGround(const std::vector<game_point>& player, const std::vector<game_line>::const_iterator groundBegin, const std::vector<game_line>::const_iterator groundEnd);
bool BulletHasHitTheGround(const bullet& bullet, const std::vector<game_line>::const_iterator groundBegin, const std::vector<game_line>::const_iterator groundEnd);

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

int ProcessBulletTargetCollisions(
  std::vector<bullet_target_collision>::iterator collisionsBegin, 
  std::vector<bullet_target_collision>::iterator collisionsEnd);

int GetGroundInterceptCount(const game_point& point, std::vector<game_line>::const_iterator groundBegin, std::vector<game_line>::const_iterator groundEnd);

#endif
