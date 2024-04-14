#pragma once

#include "enemy_object.h"

class enemy_type_1 : public enemy_object
{

public:

  enemy_type_1(POINT_2F position, int hitpoints);

  auto Update(float interval, POINT_2F target) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  
private:

  inline static constexpr float m_speed { 100 };

};
