#include "pch.h"
#include "level_collision_geometry.h"

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, level_cell& object) -> void
{
  if( object.Type() == level_cell_type::wall )
  {
    m_wallGeometries.Add(defaultObject);
  }
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, player_ship& object) -> void
{
  m_playerGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, player_bullet& object) -> void
{
  m_playerGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, enemy_type_1& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, enemy_type_2& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, enemy_type_3& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, enemy_bullet_1& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, portal& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}
