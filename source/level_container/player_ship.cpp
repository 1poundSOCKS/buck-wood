#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(const game_point& position) : m_body { position }
{
}

auto player_ship::Update(float interval, float thrust, std::optional<float> angle, std::optional<float> rotation) -> void
{
  if( m_state == state::alive )
  {
    m_previousState = m_body;

    auto realAngle = angle ? *angle : m_body.Angle();
    m_body.SetAngle(realAngle);

    auto realRotation = rotation ? *rotation : 0;
    m_body.Rotate(realRotation * interval * 20.0f);

    SetThrust(thrust);

    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
  }
}
