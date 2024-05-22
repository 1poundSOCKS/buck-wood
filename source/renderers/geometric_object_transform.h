#pragma once

#include "objects/geometric_object.h"

class geometric_object_transform
{

public:

  geometric_object_transform(const geometric_object& object)
  {
    auto objectScale = object.Scale();
    auto objectPosition = object.Position();
    auto scale = D2D1::Size(objectScale.x, objectScale.y);
    auto translation = D2D1::Size(objectPosition.x, objectPosition.y);
    m_value = D2D1::Matrix3x2F::Scale(scale) * D2D1::Matrix3x2F::Translation(translation);
  }

  [[nodiscard]] auto Get() const noexcept -> const D2D1::Matrix3x2F&
  {
    return m_value;
  }

private:

  D2D1::Matrix3x2F m_value;
};
