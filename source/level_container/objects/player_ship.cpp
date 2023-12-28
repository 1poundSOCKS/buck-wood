#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(game_point position) : m_body { position, game_velocity { 0, 0 }, 0 }, m_previousState { m_body }, m_reloadTimer { 1.0f / 2.0f }
{
}

auto player_ship::Update(float interval, float thrust, std::optional<float> angle, std::optional<float> rotation, bool shoot, update_events* updateEvents) -> void
{
  if( m_state == state::alive )
  {
    m_previousState = m_body;

    updateEvents->shot = m_reloadTimer.Update(interval) && shoot;

    auto realAngle = angle ? *angle : m_body.Angle();
    m_body.SetAngle(realAngle);

    auto realRotation = rotation ? *rotation : 0;
    m_body.Rotate(realRotation * interval * 20.0f);

    SetThrust(thrust);

    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
  }
}
