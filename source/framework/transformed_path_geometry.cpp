#include "pch.h"
#include "transformed_path_geometry.h"
#include "framework.h"

transformed_path_geometry::transformed_path_geometry(ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform)
{
  HRESULT hr = render_target::d2dFactory()->CreateTransformedGeometry(geometry, transform, m_geometry.put());

  if( FAILED(hr) )
    throw std::exception();
}

transformed_path_geometry::transformed_path_geometry(const path_geometry& geometry, const D2D1_MATRIX_3X2_F& transform)
{
  HRESULT hr = render_target::d2dFactory()->CreateTransformedGeometry(geometry.Get(), transform, m_geometry.put());

  if( FAILED(hr) )
    throw std::exception();
}

[[nodiscard]] auto transformed_path_geometry::Get() const -> ID2D1TransformedGeometry*
{
  return m_geometry.get();
}
