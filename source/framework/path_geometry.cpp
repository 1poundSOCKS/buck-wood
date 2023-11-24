#include "pch.h"
#include "path_geometry.h"

path_geometry::path_geometry(ID2D1Factory* d2dFactory)
{
  HRESULT hr = d2dFactory->CreatePathGeometry(m_geometry.put());

  if( FAILED(hr) )
  {
    throw L"error";
  }
}

auto path_geometry::Get() const -> ID2D1PathGeometry*
{
  return m_geometry.get();  
}
