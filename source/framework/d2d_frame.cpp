#include "pch.h"
#include "d2d_frame.h"

d2d_frame::d2d_frame(ID2D1RenderTarget* renderTarget)
: renderTarget(renderTarget)
{
  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}
