#include "pch.h"
#include "screen_render.h"
#include "render.h"

consteval std::array<D2D1_POINT_2F, 8> GetCursorRenderData()
{
  const float cursorSize = 20.0f;
  const float cursorSizeGap = 10.0f;

  return std::array<D2D1_POINT_2F, 8> {
    D2D1_POINT_2F { 0, -cursorSize },
    D2D1_POINT_2F { 0,-cursorSizeGap },
    D2D1_POINT_2F { 0,cursorSize },
    D2D1_POINT_2F { 0,cursorSizeGap },
    D2D1_POINT_2F { -cursorSize,0 },
    D2D1_POINT_2F { -cursorSizeGap,0 },
    D2D1_POINT_2F { cursorSize,0 },
    D2D1_POINT_2F { cursorSizeGap,0 }
  };
}

void RenderMouseCursor(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, float x, float y)
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  auto mouseCursor = GetCursorRenderData();

  std::vector<render_line> renderLines;
  
  CreateDisconnectedRenderLines(
    mouseCursor.cbegin(), 
    mouseCursor.cend(), 
    std::back_inserter(renderLines), 
    brush, 5, x, y);
  
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}

winrt::com_ptr<IDWriteFactory> CreateDWriteFactory()
{
  winrt::com_ptr<IDWriteFactory> dwriteFactory;
  HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(dwriteFactory),reinterpret_cast<IUnknown**>(dwriteFactory.put()));
  if( FAILED(hr) ) throw L"error";
  return dwriteFactory;
}

winrt::com_ptr<ID2D1SolidColorBrush> CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color)
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  HRESULT hr = renderTarget->CreateSolidColorBrush(color, brush.put());
  if( FAILED(hr) ) throw(L"error");
  return brush;
}

winrt::com_ptr<IDWriteTextFormat> CreateScreenRenderTextFormat(
  IDWriteFactory* writeFactory, 
  LPCWSTR fontFamily, 
  DWRITE_FONT_WEIGHT fontWeight, 
  DWRITE_FONT_STYLE fontStyle,
  DWRITE_FONT_STRETCH fontStretch,
  float fontSize)
{
  winrt::com_ptr<IDWriteTextFormat> textFormat;
  HRESULT hr = writeFactory->CreateTextFormat(fontFamily, NULL, fontWeight, fontStyle, fontStretch, fontSize, L"", textFormat.put());
  if( FAILED(hr) ) throw L"error";
  return textFormat;
}
