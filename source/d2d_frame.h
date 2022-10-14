#include <d2d1.h>
#include <dwrite.h>
#include <winrt/base.h>

#include "game_objects.h"

struct d2d_frame
{
  d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget);
  ~d2d_frame();

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<ID2D1SolidColorBrush> brushDiagnostics;
  winrt::com_ptr<ID2D1SolidColorBrush> brushTimer;
  winrt::com_ptr<ID2D1SolidColorBrush> brushDeactivated;
  winrt::com_ptr<ID2D1SolidColorBrush> brushActivated;
  winrt::com_ptr<IDWriteFactory> writeFactory;
  winrt::com_ptr<IDWriteTextFormat> writeTextFormat;
  winrt::com_ptr<IDWriteTextFormat> levelTimerTextFormat;
};
