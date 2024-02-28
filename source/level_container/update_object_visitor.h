#pragma once

#include "dynamic_object.h"
#include "level_geometries.h"

struct update_object_visitor
{
  dynamic_object<default_object>& m_dynamicObject;
  float m_interval;
  POINT_2F m_playerPosition;

  auto operator()(const auto& object)
  {
    m_dynamicObject.Update(m_interval, m_playerPosition);
  }
};
