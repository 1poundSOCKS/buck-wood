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

  // duct_fan(float x, float y, float size, float rotationSpeed) : 
  //   m_position { x , y }, m_rotationSpeed { rotationSpeed },
  //   m_baseGeometry { d2d_factory::get_raw(), GetGeometryData(size) }, 
  //   m_geometry { d2d_factory::get_raw(), GetGeometryData(size), D2D1::Matrix3x2F::Translation(x, y) }
  // {
  // }

  duct_fan(float x, float y, float rotationSpeed) : m_position { x , y }, m_rotationSpeed { rotationSpeed }
  {
  }

  // [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&
  // {
  //   return m_geometry;
  // }

  auto Update(float interval) -> void
  {
    m_angle += m_rotationSpeed * interval;
    // m_geometry = { d2d_factory::get_raw(), m_baseGeometry.Get(), D2D1::Matrix3x2F::Rotation(m_angle) * D2D1::Matrix3x2F::Translation(m_position.x, m_position.y) };
  }

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Angle() const -> float;

private:

  // path_geometry m_baseGeometry;
  // transformed_path_geometry m_geometry;

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
