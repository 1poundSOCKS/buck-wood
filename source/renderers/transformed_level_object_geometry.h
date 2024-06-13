#pragma once

#include "level_object_geometry.h"
#include "geometric_object_transform.h"

class transformed_level_object_geometry
{

public:

  transformed_level_object_geometry(const auto& object) : m_value { GetGeometry(object) }
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
  
  [[nodiscard]] auto Bounds() const -> RECT_F
  {
    return direct2d::GetGeometryBounds(m_value.get());
  }

private:

  static [[nodiscard]] auto GetGeometry(const auto& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    auto transform = geometric_object_transform { object };
    auto geometry = level_object_geometry { object };
    return direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), geometry.GetRaw(), transform.Get());
  }

  winrt::com_ptr<ID2D1Geometry> m_value;

};
