#include "d2d_frame.h"

d2d_frame::d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, float mouseX, float mouseY)
: renderTarget(renderTarget),
  viewTransform(viewTransform),
  renderTargetMouseX(0),
  renderTargetMouseY(0)
{
  const auto renderTargetSize = renderTarget->GetSize();
  renderTargetMouseX = mouseX * renderTargetSize.width;
  renderTargetMouseY = mouseY * renderTargetSize.height;

  auto mouseTransform = viewTransform;
  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = renderTargetMouseX;
    inPoint.y = renderTargetMouseY;
    auto outPoint = mouseTransform.TransformPoint(inPoint);
    worldMouseX = outPoint.x;
    worldMouseY = outPoint.y;
  }

  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}
