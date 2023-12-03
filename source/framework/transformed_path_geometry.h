#pragma once

#include "path_geometry.h"
#include "directx_functions.h"

class transformed_path_geometry
{

public:

  transformed_path_geometry() = default;
  transformed_path_geometry(ID2D1Factory* d2dFactory, ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform);
  transformed_path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points);
  transformed_path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points, const D2D1_MATRIX_3X2_F& transform);

  [[nodiscard]] auto Get() const -> ID2D1TransformedGeometry*;

private:

  winrt::com_ptr<ID2D1TransformedGeometry> m_geometry;

};

transformed_path_geometry::transformed_path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points)
{
  path_geometry baseGeometry { d2dFactory, points };
  m_geometry = CreateTransformedGeometry(d2dFactory, baseGeometry.Get(), D2D1::Matrix3x2F::Identity());
}


transformed_path_geometry::transformed_path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points, const D2D1_MATRIX_3X2_F& transform)
{
  path_geometry baseGeometry { d2dFactory, points };
  m_geometry = CreateTransformedGeometry(d2dFactory, baseGeometry.Get(), transform);
}
