#ifndef _screen_render_
#define _screen_render_

winrt::com_ptr<IDWriteFactory> CreateDWriteFactory();

winrt::com_ptr<ID2D1SolidColorBrush> CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color);

winrt::com_ptr<IDWriteTextFormat> CreateScreenRenderTextFormat(
  IDWriteFactory* writeFactory, 
  LPCWSTR fontFamily, 
  DWRITE_FONT_WEIGHT fontWeight, 
  DWRITE_FONT_STYLE fontStyle,
  DWRITE_FONT_STRETCH fontStretch,
  float fontSize);

void RenderMouseCursor(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, float x, float y);

#endif
