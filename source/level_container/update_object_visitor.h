#pragma once

#include "dynamic_object.h"
#include "level_geometries.h"
#include "level_container.h"

struct update_object_visitor
{
  level_container& m_levelContainer;
  dynamic_object<default_object>& m_dynamicObject;
  float m_interval;

  auto operator()(const level_target& object)
  {
    auto playerBullets = m_levelContainer.MovingObjects([](const auto& object)
    {
      return std::holds_alternative<player_bullet>(object->Get());
    });

    m_dynamicObject.Update(m_interval, m_levelContainer.PlayerState().Position(), playerBullets);
  }

  auto operator()(const auto& object)
  {
    m_dynamicObject.Update(m_interval, m_levelContainer.PlayerState().Position());
  }
};
