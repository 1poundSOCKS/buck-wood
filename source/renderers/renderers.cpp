#include "pch.h"
#include "renderers.h"

renderer* renderer::m_instance = nullptr;

auto renderer::create() -> void
{
  destroy();
  m_instance = new renderer();
}

auto renderer::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}
