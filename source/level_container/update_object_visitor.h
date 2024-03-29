#pragma once

#include "level_container.h"

struct update_object_visitor
{
  float m_interval;
  bool m_playerActive;
  enemy_movement_random& m_enemyMovementRandom;

  auto operator()(player_ship& object)
  {
    object.Update(m_interval, m_playerActive);
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
