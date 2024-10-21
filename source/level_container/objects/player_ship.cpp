#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  m_state { std::make_shared<player_ship_state>(position, scale, angle, velocity) }
{
}

auto player_ship::Initialize(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> std::shared_ptr<player_ship_state>
{
  m_state  = std::make_shared<player_ship_state>(position, scale, angle, velocity);
  return m_state;
}

auto player_ship::Position() const noexcept -> POINT_2F
{
  return m_state->Position();
}

auto player_ship::Scale() const noexcept -> SCALE_2F
{
  return m_state->Scale();
}

auto player_ship::Angle() const noexcept -> float
{
  return m_state->Angle();
}

auto player_ship::Age() const noexcept -> float
{
  return m_state->Age();
}

auto player_ship::Destroyed() const noexcept -> bool
{
  return m_state->Destroyed();
}

auto player_ship::Destroy() noexcept -> void
{
  m_state->Destroy();
}

auto player_ship::State() const -> std::shared_ptr<player_ship_state>
{
  return m_state;
}

auto player_ship::StateValue() const -> player_ship_state
{
  return *m_state;
}
