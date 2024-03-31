#include "pch.h"
#include "enemy_type_2.h"

enemy_type_2::enemy_type_2(POINT_2F position, float reloadTime, int hitpoints) : 
  base_object { position, { 1.5, 1.5 }, 0 }, m_reloadTimer { reloadTime }, m_hitpoints { hitpoints }
{
}

auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);

  switch( m_state )
  {
    case state_type::moving:
    {
      m_destination = m_destination ? m_destination : std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100});
      bool atDestination = m_destination && MoveTowards(m_speed * interval, *m_destination);
      m_destination = atDestination ? std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100})  : m_destination;
      m_actionCount += atDestination ? 1 : 0;
      bool stateChange = m_actionCount < m_movesBeforeStateChange ? false : true;
      m_state = stateChange ? state_type::shooting : m_state;
      m_actionCount = stateChange ? 0 : m_actionCount;
      break;
    }
    case state_type::shooting:
    {
      m_reloaded = m_reloadTimer.Update(interval);
      m_actionCount += m_reloaded ? 1 : 0;
      bool stateChange = m_actionCount > m_shotsBeforeStateChange ? true : false;
      m_state = stateChange ? state_type::moving : m_state;
      m_actionCount = stateChange ? 0 : m_actionCount;
      m_reloaded = stateChange ? false : m_reloaded;
      break;
    }
  }
}
