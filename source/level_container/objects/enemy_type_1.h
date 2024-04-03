#pragma once

#include "base_object.h"

class enemy_type_1 : public base_object
{

public:

  enemy_type_1(POINT_2F position);

  auto Update(float interval, POINT_2F target) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto Health() const -> float;

  auto ApplyDamage(int value) -> void;
};
