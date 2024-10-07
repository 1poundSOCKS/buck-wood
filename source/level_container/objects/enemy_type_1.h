#pragma once

#include "reload_timer.h"
#include "enemy_object.h"
#include "level_cell_collection.h"

class enemy_type_1 : public enemy_object
{

public:

  enum class type { stalker, random, turret };

  enemy_type_1(POINT_2F position, SCALE_2F scale, float angle, type enemyType);

  auto Update(float interval, POINT_2F target, level_cell_collection& cells) -> void;

  [[nodiscard]] auto Type() const noexcept -> type;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  enum class status { moving, waiting };

  auto UpdateStalker(float interval, POINT_2F target, level_cell_collection& cells) -> void;
  auto UpdateRandom(float interval, POINT_2F target, level_cell_collection& cells) -> void;
  auto UpdateTurret(float interval, POINT_2F targetPosition) noexcept -> void;
  [[nodiscard]] auto UpdateWhenMoving(float interval, level_cell_collection &cells) noexcept -> status;
  [[nodiscard]] auto UpdateWhenWaiting(float interval) noexcept -> status;

private:

  type m_type;
  status m_status;
  reload_timer m_waitTimer;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };

};
