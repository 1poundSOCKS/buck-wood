#pragma once

#include "path_geometry.h"

class transformed_path_geometry
{

public:

  transformed_path_geometry() = default;
  transformed_path_geometry(ID2D1Factory* d2dFactory, ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform);
  [[nodiscard]] auto Get() const -> ID2D1TransformedGeometry*;

private:

  winrt::com_ptr<ID2D1TransformedGeometry> m_geometry;

};
