#include "d2d_frame.h"

d2d_frame::d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const d2d_brushes_ptr& brushes, const dwrite_text_formats_ptr& textFormats)
 : renderTarget(renderTarget), brushes(brushes), textFormats(textFormats)
{
  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}
