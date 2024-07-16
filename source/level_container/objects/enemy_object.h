#pragma once

#include "base_object.h"
#include "play_events.h"
#include "level_cell_collection.h"
#include "object_cell_position.h"

class enemy_object : public base_object
{

public:

  enemy_object(cell_id position, level_cell_collection& cells) noexcept;
  enemy_object(POINT_2F position, SCALE_2F scale, float angle) noexcept;

  auto SetHitpoints(int value) noexcept -> void;
  auto ApplyDamage(int value) -> void;

  [[nodiscard]] auto Health() const -> float;

protected:

  int m_maxHitpoints;
  int m_hitpoints;
  object_cell_position m_cellPosition;

};
