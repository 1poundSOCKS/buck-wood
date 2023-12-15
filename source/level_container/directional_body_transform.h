#pragma once

#include "directional_body.h"

class directional_body_transform
{

public:

  directional_body_transform(const directional_body& body) : 
    m_transform { D2D1::Matrix3x2F::Rotation(body.Angle(), D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(body.Position().x, body.Position().y) }
  {
  }

  [[nodiscard]] auto Get() const -> const D2D1::Matrix3x2F&
  {
    return m_transform;
  }

  operator const D2D1::Matrix3x2F&()
  {
    return m_transform;
  }

private:

  D2D1::Matrix3x2F m_transform;

};
