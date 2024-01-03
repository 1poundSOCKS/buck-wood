#include "pch.h"
#include "dynamic_geometry.h"

dynamic_geometry::dynamic_geometry(const path_geometry& pathGeometry, const D2D1::Matrix3x2F& transform) : m_transformedGeometry { pathGeometry, transform }
{
}

dynamic_geometry::dynamic_geometry(const path_geometry& pathGeometry) : m_transformedGeometry { pathGeometry, D2D1::Matrix3x2F::Identity() }
{
}

auto dynamic_geometry::Transform(const D2D1::Matrix3x2F& transform) -> void
{
  m_transformedGeometry = transformed_path_geometry { m_transformedGeometry.GetSource().get(), transform };
}
