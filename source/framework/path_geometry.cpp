#include "pch.h"
#include "path_geometry.h"

path_geometry::path_geometry() : m_geometry(framework::createPathGeometry())
{
}

auto path_geometry::Get() const -> ID2D1PathGeometry*
{
  return m_geometry.get();  
}
