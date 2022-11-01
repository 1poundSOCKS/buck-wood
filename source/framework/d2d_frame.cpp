#include "d2d_frame.h"

d2d_frame::d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform)
: renderTarget(renderTarget),
  viewTransform(viewTransform),
  renderTargetMouseX(0),
  renderTargetMouseY(0)
{
  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}
