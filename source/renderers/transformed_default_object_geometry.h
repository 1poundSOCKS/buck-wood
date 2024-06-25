#pragma once

#include "default_object.h"
#include "transformed_level_object_geometry.h"

class transformed_default_object_geometry
{

public:

  transformed_default_object_geometry(const default_object& object)
  {
    SetGeometry(object);
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

  [[nodiscard]] auto SetGeometry(const default_object& object) -> void
  {
    object.Visit([this](const auto& object) { SetGeometry(object); });
  }

  [[nodiscard]] auto SetGeometry(const auto& object) -> void
  {
    transformed_level_object_geometry geometry { object };
    m_value = geometry.Get();
  }

  winrt::com_ptr<ID2D1Geometry> m_value;
};
