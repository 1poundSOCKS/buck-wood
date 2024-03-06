#pragma once

#include "level_container.h"

struct update_object_visitor
{
  level_container& m_levelContainer;
  float m_interval;

  auto operator()(player_ship& object)
  {
    object.SetInvulnerable(m_levelContainer.IsComplete());
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
