#include "pch.h"
#include "path_geometry.h"
#include "framework.h"

path_geometry::path_geometry() : m_geometry(framework::CreatePathGeometry())
{
}

path_geometry::path_geometry(const game_closed_object& object) : m_geometry(framework::CreatePathGeometry())
{
  Load(object);
}

auto path_geometry::Get() const -> ID2D1PathGeometry*
{
  return m_geometry.get();  
}

auto path_geometry::Load(const game_closed_object& object) -> void
{
  winrt::com_ptr<ID2D1GeometrySink> sink;
  m_geometry->Open(sink.put());

  sink->BeginFigure({ object.points.front().x, object.points.front().y }, D2D1_FIGURE_BEGIN_FILLED);

  for( auto point = std::next(object.points.cbegin()); point != object.points.cend(); ++point )
  {
    sink->AddLine({ point->x, point->y });
  }

  sink->EndFigure(D2D1_FIGURE_END_CLOSED);

  sink->Close();
}
