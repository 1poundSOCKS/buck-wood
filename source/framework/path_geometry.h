#pragma once

#include "d2d_factory.h"

class path_geometry
{

public:

  path_geometry(ID2D1Factory* d2dFactory);
  path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points);

  path_geometry();
  path_geometry(std::ranges::input_range auto&& points);

  auto Get() const -> ID2D1PathGeometry*;
  auto Load(std::ranges::input_range auto&& points) -> void;

  operator ID2D1Geometry*() const;

private:

  winrt::com_ptr<ID2D1PathGeometry> m_geometry;

};

path_geometry::path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points) : path_geometry(d2dFactory)
{
  Load(points);
}

path_geometry::path_geometry(std::ranges::input_range auto&& points) : path_geometry(d2d_factory::get_raw(), points)
{
}

auto path_geometry::Load(std::ranges::input_range auto&& points) -> void
{
  winrt::com_ptr<ID2D1GeometrySink> sink;
  m_geometry->Open(sink.put());

  auto begin = std::cbegin(points);

  auto firstPoint = *begin;

  sink->BeginFigure({ firstPoint.x, firstPoint.y }, D2D1_FIGURE_BEGIN_FILLED);

  for( auto next = std::next(begin); next != std::cend(points); ++next )
  {
    auto nextPoint = *next;
    
    sink->AddLine({ nextPoint.x, nextPoint.y });
  }

  sink->EndFigure(D2D1_FIGURE_END_CLOSED);

  sink->Close();
}

inline path_geometry::operator ID2D1Geometry*() const
{
  return m_geometry.get();
}
