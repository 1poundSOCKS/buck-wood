#pragma once

#include "level_objects.h"
#include "geometry/level_geometries.h"

class level_object_geometry
{

public:

  level_object_geometry(const auto& object) noexcept : m_value { GetGeometry(object) }
  {
  }

  [[nodiscard]] auto Get() const noexcept -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_value;
  }

  [[nodiscard]] auto GetRaw() const noexcept -> ID2D1Geometry*
  {
    return m_value.get();
  }

private:

  static [[nodiscard]] auto GetGeometry(const level_wall& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return level_geometries::RectangleGeometry();
  }

  static [[nodiscard]] auto GetGeometry(const enemy_bullet_1& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return level_geometries::MineGeometry();
  }

  static [[nodiscard]] auto GetGeometry(const auto& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    return level_geometries::TargetGeometry();
  }

private:

  winrt::com_ptr<ID2D1Geometry> m_value;

};
