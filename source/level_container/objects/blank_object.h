#pragma once

#include "framework.h"

class blank_object
{

public:

  blank_object(std::ranges::input_range auto&& points) : m_geometry { direct2d::CreatePathGeometry(d2d_factory::get_raw(), points) } {}

  [[nodiscard]] auto Geometry() const -> winrt::com_ptr<ID2D1PathGeometry>
  {
    return m_geometry;
  }

  operator ID2D1PathGeometry*() const
  {
    return m_geometry.get();
  }

private:

  winrt::com_ptr<ID2D1PathGeometry> m_geometry;

};
