#pragma once

#include "level_container.h"

struct save_player_state_visitor
{
  level_container& m_levelContainer;

  auto operator()(const player_ship& object)
  {
    m_levelContainer.SavePlayerState(object);
  }

  auto operator()(const auto& object)
  {
  }
};
