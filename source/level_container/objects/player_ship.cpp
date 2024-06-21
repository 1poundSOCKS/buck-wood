#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  m_state { std::make_shared<player_ship_state>(position, scale, angle) }
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

auto player_ship::UpdateVelocity(VELOCITY_2F changeInVelocity, float interval) -> void
{
  auto currentVelocity = m_state->Velocity();
  auto newVelocity = VELOCITY_2F { currentVelocity.x + changeInVelocity.x * interval, currentVelocity.y + changeInVelocity.y * interval };
  m_state->SetVelocity(newVelocity);
}

auto player_ship::UpdateAngle() -> void
{
  m_state->SetAngle(direct2d::CalculateDirection(m_state->Velocity()));
}

auto player_ship::UpdatePosition(float interval) -> POINT_2F
{
  return m_state->UpdatePosition(interval);
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

auto player_ship::UpdateWhenActive(float interval) -> void
{
  m_state->Update();
  m_playerReloadCounter.Update(interval);
}

auto player_ship::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_state->SetAngle(direct2d::RotateAngle(m_state->Angle(), rotationAmount));
}
