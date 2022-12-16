#include "pch.h"
#include "render_guard.h"

render_guard::render_guard(winrt::com_ptr<ID2D1RenderTarget> renderTarget)
: renderTarget(renderTarget)
{
  renderTarget->BeginDraw();
}

render_guard::~render_guard()
{
  renderTarget->EndDraw();
}
