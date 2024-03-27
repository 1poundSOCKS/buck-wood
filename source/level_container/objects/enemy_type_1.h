#pragma once

#include "framework.h"
#include "base_object.h"
#include "object_destination.h"
#include "enemy_movement_path.h"

class enemy_type_1 : public base_object
{

public:

  enemy_type_1(POINT_2F position);
  auto Update(float interval) -> void;

  constexpr [[nodiscard]] auto Health() const -> float;

private:

  constexpr static auto m_path { GetEnemyPath() };
  constexpr static float m_speed { 500 };
  int m_destinationIndex { 0 };

  enemy_movement_path m_pathMovement;

};

inline enemy_type_1::enemy_type_1(POINT_2F position) : base_object { position, { 1, 1 }, 0 }
{
}

inline auto enemy_type_1::Update(float interval) -> void
{
  m_position = m_pathMovement(m_position, interval);
  base_object::Update(interval);
}

constexpr [[nodiscard]] auto enemy_type_1::Health() const -> float
{
  return 1.0f;
}
