#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  m_state { position, scale, angle, velocity }
{
}

auto player_ship::Update(VELOCITY_2F environmentalForces, float airResistance, float interval) -> void
{
  m_state.Update(environmentalForces, airResistance, interval);
}

auto player_ship::Position() const noexcept -> POINT_2F
{
  return m_state.Position();
}

auto player_ship::Scale() const noexcept -> SCALE_2F
{
  return m_state.Scale();
}

auto player_ship::Angle() const noexcept -> float
{
  return m_state.Angle();
}

auto player_ship::Age() const noexcept -> float
{
  return m_state.Age();
}

auto player_ship::Destroyed() const noexcept -> bool
{
  return m_state.Destroyed();
}

auto player_ship::Destroy() noexcept -> void
{
  m_state.Destroy();
}

auto player_ship::State() const -> const player_ship_state&
{
  return m_state;
}

auto player_ship::State() -> player_ship_state&
{
  return m_state;
}

auto player_ship::StateValue() const -> player_ship_state
{
  return m_state;
}
