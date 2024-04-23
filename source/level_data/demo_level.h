#pragma once

#include "framework.h"
#include "level_boundary.h"
#include "valid_cell_collection.h"

class demo_level
{

public:

  using points_collection_i = std::list<POINT_2I>;
  using points_collection_f = std::list<POINT_2F>;

  demo_level();

  [[nodiscard]] auto BoundaryPoints() const -> const points_collection_f&;
  [[nodiscard]] auto BoundaryGeometry() const -> winrt::com_ptr<ID2D1Geometry>;

private:

  inline static constexpr int m_cellSize = 400;

  level_boundary m_boundary;
  points_collection_i m_boundaryPointsI;
  points_collection_f m_boundaryPointsF;
  winrt::com_ptr<ID2D1Geometry> m_boundaryGeometry;

};
