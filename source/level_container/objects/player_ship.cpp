#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  base_object { position, scale, angle }, m_shootAngle { 0 }, m_levelCellMovement { std::make_shared<level_cell_movement>() }
{
}

auto player_ship::Update(float interval) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      UpdateWhenActive(interval);
      break;
    case player_state::status::celebrating:
      UpdateWhenCelebrating(interval);
      break;
  }
}

auto player_ship::UpdateWhenActive(float interval) -> void
{
  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();

  m_leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( m_leftThumbstickPosition )
  {
    m_velocity.AdjustBy({ m_leftThumbstickPosition->x * m_thrustPower * interval, m_leftThumbstickPosition->y * m_thrustPower * interval });
    m_angle = direct2d::CalculateDirection(m_velocity.Get());
  }

  m_triggerDown = rightThumbstickPosition != std::nullopt;

  if( m_triggerDown )
  {
    auto shootAngle = static_cast<int>(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition));
    m_shootAngle = static_cast<float>(shootAngle);
  }

  auto moveDistance =  m_velocity.UpdatePosition({0, 0}, interval);
  auto position = POINT_2F { m_position.x + moveDistance.x, m_position.y + moveDistance.y };
  m_position = m_levelCellMovement->UpdatePosition(m_position, moveDistance, m_objectSize);
  auto collisionX = m_position.x != position.x;
  auto collisionY = m_position.y != position.y;
  auto velocityX = collisionX ? 0 : m_velocity.X();
  auto velocityY = collisionY ? 0 : m_velocity.Y();
  auto intervalFriction = 1.0f  - (m_friction * 60 * interval);
  velocityX *= intervalFriction;
  velocityY *= intervalFriction;
  m_velocity.Set(velocityX, velocityY);
}

auto player_ship::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}
