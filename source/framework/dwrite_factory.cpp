#include "pch.h"
#include "dwrite_factory.h"
#include "framework.h"
#include "directx_functions.h"

dwrite_factory* dwrite_factory::m_instance = nullptr;

auto dwrite_factory::create() -> void
{
  destroy();
  m_instance = new dwrite_factory();
}

auto dwrite_factory::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto dwrite_factory::get() -> const winrt::com_ptr<IDWriteFactory>&
{
  return m_instance->m_dwriteFactory;
}

dwrite_factory::dwrite_factory() : m_dwriteFactory { CreateDWriteFactory() }
{  
}
