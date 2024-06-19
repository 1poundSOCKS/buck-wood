#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity, std::shared_ptr<player_ship_state> state) : 
  m_state { state }, m_shootAngle { 0 }, m_levelCellMovement { std::make_shared<level_cell_movement>() }
{
  m_state->SetPosition(position);
  m_state->SetScale(scale);
  m_state->SetAngle(angle);
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

auto player_ship::UpdateWhenActive(float interval) -> void
{
  m_state->Update();
  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();

  m_leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( m_leftThumbstickPosition )
  {
    m_velocity.AdjustBy({ m_leftThumbstickPosition->x * m_thrustPower * interval, m_leftThumbstickPosition->y * m_thrustPower * interval });
    m_state->SetAngle(direct2d::CalculateDirection(m_velocity.Get()));
  }

  if( rightThumbstickPosition )
  {
    auto shootAngle = static_cast<int>(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition));
    shootAngle += 45;
    shootAngle /= 90;
    shootAngle *= 90;
    m_shootAngle = static_cast<float>(shootAngle);
  }

  auto moveDistance =  m_velocity.UpdatePosition({0, 0}, interval);
  auto initialPosition = m_state->Position();
  auto position = POINT_2F { initialPosition.x + moveDistance.x, initialPosition.y + moveDistance.y };
  auto newPosition = m_levelCellMovement->UpdatePosition(initialPosition, moveDistance, m_objectSize);
  m_state->SetPosition(newPosition);
  auto collisionX = newPosition.x != position.x;
  auto collisionY = newPosition.y != position.y;
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
  m_state->SetAngle(direct2d::RotateAngle(m_state->Angle(), rotationAmount));
}
