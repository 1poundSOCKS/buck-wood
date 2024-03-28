#pragma once

#include "level_container.h"

struct update_object_visitor
{
  std::shared_ptr<level_container> m_levelContainer;
  float m_interval;

  auto operator()(player_ship& object)
  {
    object.SetInvulnerable(m_levelContainer->PlayerInvulnerable());
    object.Update(m_interval, m_levelContainer->PlayerActive());
    m_levelContainer->SavePlayerState(object);
  }

  auto operator()(enemy_type_1& object)
  {
    object.Update(m_interval);
  }

  auto operator()(enemy_type_2& object)
  {
    object.Update(m_interval);
  }

  auto operator()(auto& object)
  {
    object.Update(m_interval);
  }
};
