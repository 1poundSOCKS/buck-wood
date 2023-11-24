#include "pch.h"
#include "path_geometry.h"
#include "screen_render.h"

path_geometry::path_geometry(ID2D1Factory* d2dFactory) : m_geometry { CreatePathGeometry(d2dFactory) }
{
}

auto path_geometry::Get() const -> ID2D1PathGeometry*
{
  return m_geometry.get();  
}
