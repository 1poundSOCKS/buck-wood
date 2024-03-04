#pragma once

#include "dynamic_object.h"
#include "level_container.h"

struct update_object_visitor
{
  level_container& m_levelContainer;
  float m_interval;

  auto operator()(player_ship& object)
  {
    object.Update(m_interval, m_levelContainer.PlayerActive());
    m_levelContainer.SavePlayerState(object);
  }

  auto operator()(level_target& object)
  {
    auto playerBullets = m_levelContainer.MovingObjects([](const auto& object)
    {
      return object->HoldsAlternative<player_bullet>();
    });

    object.Update(m_interval, m_levelContainer.PlayerState().Position(), playerBullets);
  }

  auto operator()(auto& object)
  {
    object.Update(m_interval);
  }
};
