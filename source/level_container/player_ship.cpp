#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "renderers.h"
#include "directional_body_transform.h"

player_ship::player_ship(const game_point& position) : 
  m_body { position }
{
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    m_previousState = m_body;
    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
  }
}
