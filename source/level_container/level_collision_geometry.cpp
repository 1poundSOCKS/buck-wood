#include "pch.h"
#include "level_collision_geometry.h"

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, player_ship& object) -> void
{
  m_playerGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, player_bullet& object) -> void
{
  m_playerGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, enemy_ship& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, enemy_bullet& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, portal& object) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object &defaultObject, power_up &powerUp) -> void
{
  m_enemyGeometries.Add(defaultObject);
}

auto level_collision_geometry::AddCollisionGeometry(default_object &defaultObject, boundary_walls &boundaryWalls) -> void
{
  m_boundaryGeometries.Add(defaultObject);
}
