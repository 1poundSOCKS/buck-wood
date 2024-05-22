#pragma once

#include "objects/geometric_object.h"

class geometric_object_transform
{

public:

  geometric_object_transform(const geometric_object& object)
  {
    auto scale = D2D1::Size(object.Scale().x, object.Scale().y);
    auto translation = D2D1::Size(object.Position().x, object.Position().y);
    auto transform = D2D1::Matrix3x2F::Scale(scale) * D2D1::Matrix3x2F::Translation(translation);
  }

  [[nodiscard]] auto Get() const noexcept -> const D2D1::Matrix3x2F&
  {
    return m_value;
  }

private:

  D2D1::Matrix3x2F m_value;
};
