#pragma once

#include "base_object.h"
#include "play_events.h"

class enemy_object : public base_object
{

public:

  enemy_object(POINT_2F position, SCALE_2F scale, float angle, int hitPoints) noexcept : 
    base_object { position, scale, angle }, m_maxHitpoints { hitPoints }, m_hitpoints { hitPoints }
  {
  }

  auto ApplyDamage(int value) -> void
  {
    m_hitpoints = std::max(0, m_hitpoints - value);
    m_destroyed = m_hitpoints == 0;

    if( m_destroyed )
    {
      play_events::increment(play_events::counter_type::enemies_destroyed);
    }
  }

  [[nodiscard]] auto Health() const -> float
  {
    return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
  }

protected:

  int m_maxHitpoints;
  int m_hitpoints;

};
