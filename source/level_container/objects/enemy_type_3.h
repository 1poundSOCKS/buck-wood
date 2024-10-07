#pragma once

#include "enemy_object.h"
#include "reload_timer.h"

class enemy_type_3 : public enemy_object
{

public:

  enemy_type_3(POINT_2F position, SCALE_2F scale, float angle);
  
  auto Update(float interval, POINT_2F targetPosition) noexcept -> void;
  
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;

private:

  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  std::optional<POINT_2F> m_target;

};
