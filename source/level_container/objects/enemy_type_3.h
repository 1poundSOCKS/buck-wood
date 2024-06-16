#pragma once

#include "enemy_object.h"
#include "reload_timer.h"

class enemy_type_3 : public enemy_object
{

public:

  enemy_type_3(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);
  
  auto Update(float interval) noexcept -> void;
  auto Update(float interval, POINT_2F targetPosition) noexcept -> void;
  
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;

  auto SetTarget(POINT_2F value) noexcept -> void;

private:

  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  std::optional<POINT_2F> m_target;

};
