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

auto render_target::RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_renderTarget->DrawText(text.data(), static_cast<UINT32>(text.length()), textFormat, rect, brush);
}

auto render_target::RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  D2D1_SIZE_F rect = m_renderTarget->GetSize();
  RenderText(D2D1_RECT_F { 0, 0, rect.width - 1, rect.height - 1 }, brush, textFormat, text);
}
