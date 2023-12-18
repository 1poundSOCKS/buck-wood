#pragma once

#include "transformed_path_geometry.h"

class duct_fan
{

public:

  duct_fan(float x, float y, float rotationSpeed) : m_position { x , y }, m_rotationSpeed { rotationSpeed }
  {
  }

  auto Update(float interval) -> void
  {
    m_angle += m_rotationSpeed * interval;
  }

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Angle() const -> float;

private:

  game_point m_position;
  game_angle m_angle;
  float m_rotationSpeed { 0 };

};

inline [[nodiscard]] auto duct_fan::Position() const -> const game_point&
{
  return m_position;
}

inline [[nodiscard]] auto duct_fan::Angle() const -> float
{
  return m_angle;
}
