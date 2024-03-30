#pragma once

#include "level_container.h"

struct update_object_visitor
{
  level_container* m_levelContainer;
  float m_interval;

  auto operator()(player_ship& object)
  {
    m_levelContainer->UpdateObject(object, m_interval);
  }

  auto operator()(enemy_type_1& object)
  {
    m_levelContainer->UpdateObject(object, m_interval);
  }

  auto operator()(enemy_type_2& object)
  {
    m_levelContainer->UpdateObject(object, m_interval);
  }

  auto operator()(auto&& object)
  {
    object.Update(m_interval);
  }
};
