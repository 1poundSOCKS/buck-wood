#pragma once

// #include "geometry.h"
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

path_geometry::path_geometry(std::ranges::input_range auto&& points) : m_geometry(framework::CreatePathGeometry())
{
  Load(points);
}

auto path_geometry::Load(std::ranges::input_range auto&& points) -> void
{
  winrt::com_ptr<ID2D1GeometrySink> sink;
  m_geometry->Open(sink.put());

  auto begin = std::cbegin(points);

  sink->BeginFigure({ begin->x, begin->y }, D2D1_FIGURE_BEGIN_FILLED);

  for( auto point = std::next(begin); point != std::cend(points); ++point )
  {
    sink->AddLine({ point->x, point->y });
  }

  sink->EndFigure(D2D1_FIGURE_END_CLOSED);

  sink->Close();
}
