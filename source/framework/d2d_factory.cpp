#include "pch.h"
#include "d2d_factory.h"
#include "directx_functions.h"

auto d2d_factory::create() -> void
{
  destroy();
  m_instance = new d2d_factory();
}

d2d_factory::d2d_factory() : com_singleton(CreateD2DFactory())
{
}
