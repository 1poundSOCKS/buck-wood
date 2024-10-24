#pragma once

#include "objects/player_ship_state.h"

namespace player_controls
{

  auto Update(std::shared_ptr<player_ship_state> state, float interval) noexcept -> void;

};
