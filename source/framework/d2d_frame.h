#ifndef _d2d_frame_
#define _d2d_frame_

#include <d2d1.h>
#include <dwrite.h>
#include <winrt/base.h>

struct d2d_frame
{
  d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform);
  ~d2d_frame();

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  D2D1::Matrix3x2F viewTransform;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
};

#endif
