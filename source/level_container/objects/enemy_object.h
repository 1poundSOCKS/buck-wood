#pragma once

#include "base_object.h"
#include "play_events.h"

class enemy_object : public base_object
{

public:

  enemy_object(POINT_2F position, SCALE_2F scale, float angle, int hitpoints) noexcept;

  auto ApplyDamage(int value) -> void;

  [[nodiscard]] auto Health() const -> float;

  auto Update(float interval) noexcept -> void;

  [[nodiscard]] auto RemainingDamageInterval() const noexcept -> float;

protected:

  constexpr static float m_startingDamageInterval { 0.2 };
  int m_maxHitpoints;
  int m_hitpoints;
  float m_remainingDamageInterval;

};
