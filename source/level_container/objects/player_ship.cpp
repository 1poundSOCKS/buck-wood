#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(player_ship_state& state) : 
  m_state { state }
{
}

auto player_ship::Update(VELOCITY_2F environmentalForces, float airResistance, float interval, bool levelComplete) -> void
{
  m_state.Update(environmentalForces, airResistance, interval, levelComplete);
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
