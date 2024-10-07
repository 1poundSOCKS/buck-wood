#pragma once

#include "enemy_object.h"
#include "level_cell_collection.h"

class enemy_type_1 : public enemy_object
{

public:

  enemy_type_1(POINT_2F position, SCALE_2F scale, float angle);

  auto Update(float interval, POINT_2F target, level_cell_collection& cells) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

};
