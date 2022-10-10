#include <d2d1.h>
#include <dwrite.h>
#include <winrt/base.h>

struct d2d_frame
{
  d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float yOffset);
  ~d2d_frame();

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<IDWriteFactory> writeFactory;
  winrt::com_ptr<IDWriteTextFormat> writeTextFormat;
  D2D1::Matrix3x2F transform;
};
