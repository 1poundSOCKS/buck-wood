#include "pch.h"
#include "transformed_path_geometry.h"
#include "framework.h"

transformed_path_geometry::transformed_path_geometry(ID2D1Factory* d2dFactory, ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform) :
  m_geometry { CreateTransformedGeometry(d2dFactory, geometry, transform) }
{
}

transformed_path_geometry::transformed_path_geometry(ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform) :
  m_geometry { CreateTransformedGeometry(d2d_factory::get_raw(), geometry, transform) }
{
}

[[nodiscard]] auto transformed_path_geometry::Get() const -> ID2D1TransformedGeometry*
{
  return m_geometry.get();
}
