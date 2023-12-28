#include "pch.h"
#include "mine.h"

mine::data::data(game_point position) : m_body { position, game_velocity { 0, 0 }, m_thrustPower }, m_previousState { m_body }
{
}

auto mine::data::Update(float interval, std::optional<game_point> playerPosition) -> void
{
  m_previousState = m_body;
  m_body.Update(interval, playerPosition);
  m_spin += m_spinRate * interval;
}

mine::mine(game_point position) : m_data { std::make_shared<data>(position)}
{
}

auto mine::Update(float interval, std::optional<game_point> playerPosition) -> void
{
  m_data->Update(interval, playerPosition);
}

[[nodiscard]] auto mine::Destroyed() const -> bool
{
  return m_data->m_destroyed;
}

auto mine::Destroy() -> void
{
  m_data->m_destroyed = true;
}
