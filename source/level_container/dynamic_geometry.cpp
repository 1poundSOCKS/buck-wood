#include "pch.h"
#include "dynamic_geometry.h"

auto dynamic_geometry::Transform(const D2D1::Matrix3x2F& transform) -> void
{
  auto sourceGeometry = m_transformedGeometry.GetSource();
  m_transformedGeometry = transformed_path_geometry { sourceGeometry.get(), transform };
}
