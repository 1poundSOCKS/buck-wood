#pragma once

#include "framework.h"

class path_geometry
{

public:

  path_geometry();
  path_geometry(std::ranges::input_range auto&& points);

  auto Get() const -> ID2D1PathGeometry*;
  auto Load(std::ranges::input_range auto&& points) -> void;

private:

  winrt::com_ptr<ID2D1PathGeometry> m_geometry;

};

path_geometry::path_geometry(std::ranges::input_range auto&& points) : m_geometry(framework::createPathGeometry())
{
  Load(points);
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
