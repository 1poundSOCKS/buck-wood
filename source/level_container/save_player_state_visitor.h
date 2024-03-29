#pragma once

#include "level_container.h"

struct save_player_state_visitor
{
  std::shared_ptr<level_container> m_levelContainer;

  auto operator()(const player_ship& object)
  {
    m_levelContainer->SavePlayerState(object);
  }

  auto operator()(auto&& object)
  {
  }
};
