#ifndef _d2d_brushes_
#define _d2d_brushes_

#include <winrt/base.h>
#include <d2d1.h>

struct d2d_brushes
{
  d2d_brushes(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget);

  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<ID2D1SolidColorBrush> brushThrusters;
  winrt::com_ptr<ID2D1SolidColorBrush> brushDiagnostics;
  winrt::com_ptr<ID2D1SolidColorBrush> brushTimer;
  winrt::com_ptr<ID2D1SolidColorBrush> brushDeactivated;
  winrt::com_ptr<ID2D1SolidColorBrush> brushActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> brushLevelEndText;
};

using d2d_brushes_ptr = std::shared_ptr<d2d_brushes>;

#endif
