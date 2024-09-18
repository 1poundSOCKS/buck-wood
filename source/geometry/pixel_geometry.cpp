#include "pch.h"
#include "framework.h"
#include "pixel_geometry.h"
#include "shape_generator.h"

pixel_geometry::operator winrt::com_ptr<ID2D1Geometry>() const noexcept
{
  return m_geometry;
}

auto pixel_geometry::LoadGeometry(const pixel_id_lookup &pixelIdLookup, cell_size pixelSize) noexcept -> void
{
  std::map<POINT_2I, POINT_2I> pixelLines;

  for( auto pixelId : pixelIdLookup )
  {
    auto leftId = pixelId.Get(cell_id::relative_position::left);
    auto aboveId = pixelId.Get(cell_id::relative_position::above);
    auto rightId = pixelId.Get(cell_id::relative_position::right);
    auto belowId = pixelId.Get(cell_id::relative_position::below);

    auto pixelPosition = pixelId.Position(1, 1);

    if( !pixelIdLookup.contains(leftId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x, pixelPosition.y + 1 };
      auto pixelLineEnd = POINT_2I { pixelPosition.x, pixelPosition.y };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(aboveId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(rightId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y };
      auto pixelLineEnd = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }

    if( !pixelIdLookup.contains(belowId) )
    {
      auto pixelLineStart = POINT_2I { pixelPosition.x + 1, pixelPosition.y + 1 };
      auto pixelLineEnd = POINT_2I { pixelPosition.x, pixelPosition.y + 1 };
      pixelLines[pixelLineStart] = pixelLineEnd;
    }
  }

  std::list<POINT_2F> points;

  auto currentLine = std::begin(pixelLines);
  
  while( points.size() < pixelLines.size() )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = pixelSize.CellRect({pixelLineStart.x, pixelLineStart.y});
    auto pixelRectFloat = ToFloat(pixelRect);
    points.emplace_back(pixelRectFloat.left, pixelRectFloat.top);
    currentLine = pixelLines.find(pixelLineEnd);
  }

  m_geometry = direct2d::CreatePathGeometry(d2d_factory::get_raw(), points, D2D1_FIGURE_END_CLOSED);
}
