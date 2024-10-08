#pragma once

#include "objects/player_ship_state.h"
#include "level_cell_collection.h"

class player_controls
{

public:

  player_controls(std::shared_ptr<player_ship_state> state) noexcept;
  auto SetState(std::shared_ptr<player_ship_state> state) noexcept -> void;
  auto Update(float interval) const noexcept -> void;

private:

  std::shared_ptr<player_ship_state> m_state;

};
