#pragma once

#include "framework.h"

class blank_object
{

public:

  blank_object(std::ranges::input_range auto&& points)
  {
    if( !std::ranges::empty(points) )
    {
      m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
    }
  }

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
