#include "pch.h"
#include "level_geometry_scale.h"
#include "level_geometries.h"
#include "direct2d_functions.h"

level_geometry_scale::level_geometry_scale() : m_playerScale { Scale(level_geometries::player().get(), { 100, 100 }) }
{
}

auto level_geometry_scale::Scale(ID2D1Geometry *geometry, SIZE_F objectSize) -> SCALE_2F
{
  auto bounds = direct2d::GetGeometryBounds(geometry);
  auto geometryWidth = bounds.right - bounds.left;
  auto geometryHeight = bounds.bottom - bounds.top;
  return SCALE_2F( objectSize.width / geometryWidth, objectSize.height / geometryHeight );
}
