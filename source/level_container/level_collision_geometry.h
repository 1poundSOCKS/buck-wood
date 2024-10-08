#pragma once

#include "default_object_geometry_collection_range.h"

class level_collision_geometry
{

public:

  enum class type { player, enemy, wall, boundary };

  level_collision_geometry() = default;
  auto Update(std::ranges::input_range auto&& objects) -> void;
  
  [[nodiscard]] auto operator()(type geometryType) -> default_object_geometry_collection_range&;
  [[nodiscard]] auto operator()(type geometryType) const -> const default_object_geometry_collection_range&;

private:

  auto AddCollisionGeometry(default_object& defaultObject, level_cell& object) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, player_ship& object) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, player_bullet& object) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, enemy_ship& object) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, enemy_bullet& object) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, portal& object) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, power_up& powerUp) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, boundary_walls& boundaryWalls) -> void;
  auto AddCollisionGeometry(default_object& defaultObject, auto& object) -> void;

private:

  default_object_geometry_collection m_playerGeometries;
  default_object_geometry_collection m_enemyGeometries;
  default_object_geometry_collection m_wallGeometries;
  default_object_geometry_collection m_boundaryGeometries;

  default_object_geometry_collection_range m_playerGeometryRange { m_playerGeometries };
  default_object_geometry_collection_range m_enemyGeometryRange { m_enemyGeometries };
  default_object_geometry_collection_range m_wallGeometryRange { m_wallGeometries };
  default_object_geometry_collection_range m_boundaryGeometryRange { m_boundaryGeometries };

};

auto level_collision_geometry::Update(std::ranges::input_range auto &&objects) -> void
{
  m_playerGeometries.Clear();
  m_enemyGeometries.Clear();
  m_wallGeometries.Clear();
  m_boundaryGeometries.Clear();
  
  for( auto& object : objects )
  {
    object.Visit([this,&object](auto& levelObject) { AddCollisionGeometry(object, levelObject); });
  }
}

inline auto level_collision_geometry::operator()(type geometryType) -> default_object_geometry_collection_range &
{
  switch( geometryType )
  {
    case type::player:
      return m_playerGeometryRange;
    case type::enemy:
      return m_enemyGeometryRange;
    case type::boundary:
      return m_boundaryGeometryRange;
    default:
      return m_wallGeometryRange;
  }
}

inline auto level_collision_geometry::operator()(type geometryType) const -> const default_object_geometry_collection_range &
{
  switch( geometryType )
  {
    case type::player:
      return m_playerGeometryRange;
    case type::enemy:
      return m_enemyGeometryRange;
    case type::boundary:
      return m_boundaryGeometryRange;
    default:
      return m_wallGeometryRange;
  }
}

auto level_collision_geometry::AddCollisionGeometry(default_object& defaultObject, auto &object) -> void
{
}
