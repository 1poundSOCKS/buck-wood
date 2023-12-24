#pragma once

#include "transformed_path_geometry.h"
#include "game_scale.h"

class duct_fan
{

public:

  duct_fan(float x, float y, float rotationSpeed) : m_position { x , y }, m_rotationSpeed { rotationSpeed }
  {
  }

  [[nodiscard]] auto Scale() const -> game_scale { return m_scale; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;

  auto Update(float interval) -> void
  {
    m_scale += game_scale { 0.002f, 0.0f };
    m_angle += m_rotationSpeed * interval;
  }

private:

  game_scale m_scale { 0.5f, 1.0f };
  game_angle m_angle;
  game_point m_position;
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
