#include "pch.h"
#include "render_target.h"
#include "directx_functions.h"
#include "dwrite_factory.h"

render_target* render_target::m_instance = nullptr;

auto render_target::create(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> void
{
  m_instance = new render_target(swapChain, d2dFactory);
}

auto render_target::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

render_target::render_target(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) : m_renderTarget { CreateRenderTarget(swapChain, d2dFactory) }
{  
}
