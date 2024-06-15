#pragma once

#include "objects/geometric_object.h"

class geometric_object_transform
{

public:

  geometric_object_transform(const auto& object);

  [[nodiscard]] auto CreateGeometry(ID2D1Factory* factory, ID2D1Geometry* geometry) -> winrt::com_ptr<ID2D1TransformedGeometry>;
  [[nodiscard]] auto Get() const noexcept -> const D2D1::Matrix3x2F&;

private:

  D2D1::Matrix3x2F m_value;
};

geometric_object_transform::geometric_object_transform(const auto& object)
{
  auto objectScale = object.Scale();
  auto objectAngle = object.Angle();
  auto objectPosition = object.Position();
  auto scale = D2D1::Size(objectScale.x, objectScale.y);
  auto translation = D2D1::Size(objectPosition.x, objectPosition.y);
  m_value = D2D1::Matrix3x2F::Scale(scale) * D2D1::Matrix3x2F::Rotation(objectAngle) * D2D1::Matrix3x2F::Translation(translation);
}

inline auto geometric_object_transform::CreateGeometry(ID2D1Factory* factory, ID2D1Geometry* geometry) -> winrt::com_ptr<ID2D1TransformedGeometry>
{
  return direct2d::CreateTransformedGeometry(factory, geometry, m_value);
}

inline auto geometric_object_transform::Get() const noexcept -> const D2D1::Matrix3x2F&
{
  return m_value;
}
