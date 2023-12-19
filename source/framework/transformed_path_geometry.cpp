#include "pch.h"
#include "transformed_path_geometry.h"
#include "framework.h"

transformed_path_geometry::transformed_path_geometry(ID2D1Factory* d2dFactory, ID2D1Geometry* geometry, const D2D1_MATRIX_3X2_F& transform) :
  m_geometry { CreateTransformedGeometry(d2dFactory, geometry, transform) }
{
}

transformed_path_geometry::transformed_path_geometry(ID2D1Geometry* geometry, const D2D1_MATRIX_3X2_F& transform) :
  m_geometry { CreateTransformedGeometry(d2d_factory::get_raw(), geometry, transform) }
{
}

[[nodiscard]] auto transformed_path_geometry::Get() const -> ID2D1TransformedGeometry*
{
  return m_geometry.get();
}

[[nodiscard]] auto transformed_path_geometry::GetSource() const -> winrt::com_ptr<ID2D1Geometry>
{
  winrt::com_ptr<ID2D1Geometry> sourceGeometry;
  m_geometry->GetSourceGeometry(sourceGeometry.put());
  return sourceGeometry;
}

[[nodiscard]] auto transformed_path_geometry::HasCollidedWith(const transformed_path_geometry& geometry) const -> bool
{
  D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
  HRESULT hr = m_geometry->CompareWithGeometry(geometry.m_geometry.get(), D2D1::Matrix3x2F::Identity(), &relation);

  bool collided = false;

  if( SUCCEEDED(hr) )
  {
    switch( relation )
    {
      case D2D1_GEOMETRY_RELATION_IS_CONTAINED:
      case D2D1_GEOMETRY_RELATION_CONTAINS:
      case D2D1_GEOMETRY_RELATION_OVERLAP:
        collided = true;
        break;
    }
  }

  return collided;
}

[[nodiscard]] auto transformed_path_geometry::HasCollidedWith(const game_point& point) const -> bool
{
  BOOL collision = FALSE;
  HRESULT hr = m_geometry->FillContainsPoint({point.x, point.y}, D2D1::Matrix3x2F::Identity(), &collision);
  return SUCCEEDED(hr) && collision ? true : false;
}
