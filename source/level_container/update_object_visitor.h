#pragma once

#include "level_container.h"

struct update_object_visitor
{
  std::shared_ptr<level_container> m_levelContainer;
  float m_interval;
  enemy_movement_random& m_enemyMovementRandom;

  auto operator()(player_ship& object)
  {
    object.SetInvulnerable(m_levelContainer->PlayerInvulnerable());
    object.Update(m_interval, m_levelContainer->PlayerActive());
  }

  auto operator()(enemy_type_1& object)
  {
    object.Update(m_interval);
  }

  auto operator()(enemy_type_2& object)
  {
    object.Update(m_interval);
    auto [position, destination] = m_enemyMovementRandom(object.Position(), object.Destination(), m_interval);
    object.SetPosition(position);
    object.SetDestination(destination);
  }

  auto operator()(auto&& object)
  {
    object.Update(m_interval);
  }
};
