#include "pch.h"
#include "path_geometry.h"
#include "directx_functions.h"

path_geometry::path_geometry(ID2D1Factory* d2dFactory) : m_geometry { CreatePathGeometry(d2dFactory) }
{
}

path_geometry::path_geometry() : m_geometry { CreatePathGeometry(d2d_factory::get_raw()) }
{
}

auto path_geometry::Get() const -> ID2D1PathGeometry*
{
  return m_geometry.get();  
}
