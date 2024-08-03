#pragma once

#include "enemy_object.h"
#include "reload_timer.h"
#include "level_cell_collection.h"

class enemy_type_2 : public enemy_object
{

public:

  enemy_type_2(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto Update(float interval, POINT_2F targetPosition, level_cell_collection& cells) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  enum class status { moving, waiting };

  [[nodiscard]] auto Destination() const -> std::optional<level_cell_item>;
  [[nodiscard]] auto UpdateWhenMoving(float interval, level_cell_collection& cells) noexcept -> status;
  [[nodiscard]] auto UpdateWhenWaiting(float interval) noexcept -> status;

private:

  status m_status;
  reload_timer m_waitTimer;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };

};

inline [[nodiscard]] auto enemy_type_2::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}

inline [[nodiscard]] auto enemy_type_2::Reloaded() const -> bool
{
  return m_reloaded;
}
