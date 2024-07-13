#pragma once

#include "enemy_object.h"
#include "level_cell_collection.h"

class enemy_type_1 : public enemy_object
{

public:

  enemy_type_1(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto Update(float interval, POINT_2F target, level_cell_collection& cells) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  auto MoveTowardsDestination(cell_id destination, float interval, level_cell_collection& cells) noexcept -> bool;
  auto NewDestination(POINT_2F target, const level_cell_collection &cells) -> std::optional<cell_id>;

private:

  inline static constexpr float m_speed { 200 };
  std::optional<cell_id> m_destination;

};
