#include "pch.h"
#include "d2d_factory.h"
#include "directx_functions.h"

d2d_factory::d2d_factory() : m_factory { CreateD2DFactory() }
{
  
}

[[nodiscard]] auto d2d_factory::createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
{
  return CreatePathGeometry(m_instance->m_factory.get());
}
