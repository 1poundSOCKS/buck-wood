#include "pch.h"
#include "level_control_data.h"

level_control_data::level_control_data() :
  m_playerControl(std::make_shared<player_ship::control>()), m_timerControl(std::make_shared<level_timer::control>()), m_stateControl(std::make_shared<level_state::control>())
{
}

level_control_data::level_control_data(const player_ship::control_data playerControl, const level_timer::control_data timerControl, const level_state::control_data stateControl)
{
  Attach(playerControl);
  Attach(timerControl);
  Attach(stateControl);
}

auto level_control_data::Attach(const player_ship::control_data& playerControl) -> void
{
  m_playerControl = playerControl;
}

auto level_control_data::Attach(const level_timer::control_data& timerControl) -> void
{
  m_timerControl = timerControl;
}

auto level_control_data::Attach(const level_state::control_data& stateControl) -> void
{
  m_stateControl = stateControl;
}

auto level_control_data::operator=(const level_control_data& levelControlData) -> const level_control_data&
{
  Attach(levelControlData.m_playerControl);
  Attach(levelControlData.m_timerControl);
  Attach(levelControlData.m_stateControl);
  return *this;
}

[[nodiscard]] auto level_control_data::GetPlayerControl() const -> const player_ship::control_data
{
  return m_playerControl;
}

[[nodiscard]] auto level_control_data::GetTimerControl() const -> const level_timer::control_data
{
  return m_timerControl;
}

[[nodiscard]] auto level_control_data::GetStateControl() const -> const level_state::control_data
{
  return m_stateControl;
}
