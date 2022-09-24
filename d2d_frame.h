#include <d2d1.h>
#include <dwrite.h>
#include <winrt/base.h>

struct d2d_frame
{
  d2d_frame(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget);
  ~d2d_frame();

  winrt::com_ptr<ID2D1HwndRenderTarget> renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<IDWriteFactory> writeFactory;
  winrt::com_ptr<IDWriteTextFormat> writeTextFormat;
};
