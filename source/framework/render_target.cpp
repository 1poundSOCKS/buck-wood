#include "pch.h"
#include "render_target.h"
#include "directx_functions.h"
#include "dwrite_factory.h"

auto render_target::create(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> void
{
  m_instance = new render_target(swapChain, d2dFactory);
}

render_target::render_target(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) : 
  com_singleton(CreateRenderTarget(swapChain, d2dFactory))
{
}
