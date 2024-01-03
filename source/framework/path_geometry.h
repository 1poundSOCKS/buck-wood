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

path_geometry::path_geometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points) : m_geometry { CreatePathGeometry(d2dFactory, points) }
{
}

path_geometry::path_geometry(std::ranges::input_range auto&& points) : m_geometry { CreatePathGeometry(d2d_factory::get_raw(), points) }
{
}

auto path_geometry::Load(std::ranges::input_range auto&& points) -> void
{
  LoadPathGeometry(m_geometry.get(), points);
}

inline path_geometry::operator ID2D1Geometry*() const
{
  return m_geometry.get();
}
