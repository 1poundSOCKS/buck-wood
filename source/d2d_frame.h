#ifndef _d2d_frame_
#define _d2d_frame_

#include <d2d1.h>
#include <dwrite.h>
#include <winrt/base.h>

#include "d2d_brushes.h"
#include "dwrite_text_formats.h"

struct d2d_frame
{
  d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const d2d_brushes_ptr& brushes, const dwrite_text_formats_ptr& textFormats);
  ~d2d_frame();

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  d2d_brushes_ptr brushes;
  dwrite_text_formats_ptr textFormats;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
};

#endif
