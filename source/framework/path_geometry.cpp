#include "pch.h"
#include "path_geometry.h"

path_geometry::path_geometry() : m_geometry(framework::CreatePathGeometry())
{
}

// path_geometry::path_geometry(const game_closed_object& object) : m_geometry(framework::CreatePathGeometry())
// {
//   Load(object.points);
// }

auto path_geometry::Get() const -> ID2D1PathGeometry*
{
  return m_geometry.get();  
}
