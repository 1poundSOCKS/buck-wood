#include "pch.h"
#include "player_ship_state.h"
#include "player_state.h"

auto player_ship_state::Update(float interval) -> void
{
  switch(  player_state::get_status() )
  {
    case player_state::status::active:
      UpdateWhenActive(interval);
      break;
    case  player_state::status::celebrating:
      UpdateWhenCelebrating(interval);
      break;
  }
}

auto player_ship_state::UpdateWhenActive(float interval) -> void
{
  base_object::Update(interval);
  
  m_playerReloadCounter.Update(interval);
}

auto player_ship_state::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}
