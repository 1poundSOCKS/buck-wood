#include "pch.h"
#include "d2d_factory.h"
#include "direct2d_functions.h"

auto d2d_factory::create() -> void
{
  destroy();
  m_instance = new d2d_factory();
}

d2d_factory::d2d_factory() : com_singleton(direct2d::CreateD2DFactory())
{
}
