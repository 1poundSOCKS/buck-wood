#pragma once

#include "transformed_path_geometry.h"

class duct_fan
{

public:

  static [[nodiscard]] auto GetGeometryData(float size)
  {
    return m_geometryData | std::ranges::views::transform([size](const game_point& gamePoint) -> game_point
    {
      return { gamePoint.x * size, gamePoint.y * size  };
    });
  }

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

  inline static auto m_geometryData = std::array {
    game_point { -0.05, -0.5 },
    game_point { 0.05, -0.5 },
    game_point { 0.05, 0.5 },
    game_point { -0.05, 0.5 }
  };

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
