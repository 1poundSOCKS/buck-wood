#pragma once

#include "framework.h"
#include "level_boundary.h"
#include "valid_cell_collection.h"

class demo_level
{

public:

  using points_collection = std::list<POINT_2F>;

  demo_level();

  [[nodiscard]] auto BoundaryPoints() const -> const points_collection&;
  [[nodiscard]] auto BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>;

private:

  inline static constexpr int m_cellSize = 400;

  level_boundary m_boundary;
  points_collection m_boundaryPoints;
  winrt::com_ptr<ID2D1Geometry> m_boundaryGeometry;

};
