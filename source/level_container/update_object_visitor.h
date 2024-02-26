#pragma once

#include "dynamic_object.h"
#include "level_geometries.h"

struct update_object_visitor
{
  dynamic_object<default_object>& m_dynamicObject;
  float m_interval;
  POINT_2F m_playerPosition;

  auto operator()(const level_target& object)
  {
    m_dynamicObject.Update(level_geometries::TargetGeometry(), m_interval, m_playerPosition);
  }

  auto operator()(const player_ship& object)
  {
    m_dynamicObject.Update(level_geometries::PlayerShipGeometry(), m_interval, m_playerPosition);
  }

  auto operator()(const mine& object)
  {
    m_dynamicObject.Update(level_geometries::MineGeometry(), m_interval, m_playerPosition);
  }

  auto operator()(const auto& object)
  {
  }
};
