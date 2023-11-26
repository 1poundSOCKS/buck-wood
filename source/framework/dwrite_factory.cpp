#include "pch.h"
#include "dwrite_factory.h"
#include "framework.h"
#include "directx_functions.h"

auto dwrite_factory::create() -> void
{
  destroy();
  m_instance = new dwrite_factory();
}

dwrite_factory::dwrite_factory() : com_singleton { CreateDWriteFactory() }
{  
}
