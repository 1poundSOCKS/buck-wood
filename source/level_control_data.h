#pragma once

#include "player_ship.h"
#include "level_timer.h"
#include "level_state.h"

class level_control_data
{
public:

  level_control_data();
  level_control_data(const player_ship::control_data playerControl, const level_timer::control_data timerControl, const level_state::control_data stateControl);

  auto Attach(const player_ship::control_data& playerControl) -> void;
  auto Attach(const level_timer::control_data& levelControl) -> void;
  auto Attach(const level_state::control_data& stateControl) -> void;

  auto operator=(const level_control_data& levelControl) -> const level_control_data&;

  [[nodiscard]] auto GetPlayerControl() const -> const player_ship::control_data;
  [[nodiscard]] auto GetTimerControl() const -> const level_timer::control_data;
  [[nodiscard]] auto GetStateControl() const -> const level_state::control_data;

private:

  player_ship::control_data m_playerControl;
  level_timer::control_data m_timerControl;
  level_state::control_data m_stateControl;
};
