#include "pch.h"
#include "player_bullet.h"
#include "geometry/level_geometry_scale.h"

// player_bullet::player_bullet(POINT_2F position, VELOCITY_2F velocity, int damage) : 
//   base_object { position, level_geometry_scale::get(*this), direct2d::CalculateDirection(velocity) }, 
//   m_velocity { velocity }, m_damage { damage }
player_bullet::player_bullet(POINT_2F position, SCALE_2F scale, float angle) : 
  base_object { position, scale, angle }, 
  m_velocity { 0, 0 }, m_damage { 1 }
{
}
