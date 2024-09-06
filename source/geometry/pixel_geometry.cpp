#include "pch.h"
#include "framework.h"
#include "pixel_geometry.h"
#include "shape_generator.h"

pixel_geometry::pixel_geometry()
{
  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 8 }, D2D1_FIGURE_END_CLOSED);
}

pixel_geometry::operator winrt::com_ptr<ID2D1Geometry>() const
{
  return m_geometry;
}
