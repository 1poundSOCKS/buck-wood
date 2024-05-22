#pragma once

#include "enemy_object.h"
#include "level_cell_collection.h"

class enemy_type_1 : public enemy_object
{

public:

  enemy_type_1(POINT_2F position, int hitpoints);

  auto Update(float interval) -> void
  {
    base_object::Update(interval);
  }

  auto Update(float interval, POINT_2F target, const level_cell_collection& cells) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  
private:

  auto MoveTowardsDestination(valid_cell destination, float interval) noexcept -> bool;
  auto NewDestination(POINT_2F target, const level_cell_collection &cells) -> std::optional<valid_cell>;

private:

  inline static constexpr float m_speed { 200 };
  std::optional<valid_cell> m_destination;

};
