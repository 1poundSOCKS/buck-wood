#include "pch.h"
#include "framework.h"
#include "pixel_geometry.h"
#include "shape_generator.h"

pixel_geometry::operator winrt::com_ptr<ID2D1Geometry>() const
{
  return m_geometry;
}
