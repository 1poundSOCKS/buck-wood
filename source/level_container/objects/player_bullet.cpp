#include "pch.h"
#include "player_bullet.h"

player_bullet::player_bullet(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  base_object { position, scale, angle }, 
  m_velocity { velocity }, m_damage { 1 }
{
}
