#include "pch.h"
#include "screens.h"

auto screen_render_brush_white = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
auto screen_render_brush_grey = D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f);
auto screen_render_brush_green = D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f);
auto screen_render_brush_red = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
auto screen_render_brush_yellow = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
auto screen_render_brush_cyan = D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f);

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, std::back_insert_iterator<screen_render_brushes> brushInserter);
void CreateScreenRenderTextFormats(std::back_insert_iterator<screen_render_text_formats> textFormatInserter);

winrt::com_ptr<ID2D1SolidColorBrush> CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color);

winrt::com_ptr<IDWriteTextFormat> CreateScreenRenderTextFormat(
  IDWriteFactory* writeFactory, 
  LPCWSTR fontFamily, 
  DWRITE_FONT_WEIGHT fontWeight, 
  DWRITE_FONT_STYLE fontStyle,
  DWRITE_FONT_STRETCH fontStretch,
  float fontSize);

void RenderMouseCursor(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, float x, float y)
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  std::vector<D2D1_POINT_2F> mouseCursor;
  mouseCursor.reserve(8);
  mouseCursor.emplace_back(D2D1_POINT_2F(0,-cursorSize));
  mouseCursor.emplace_back(D2D1_POINT_2F(0,-cursorSizeGap));
  mouseCursor.emplace_back(D2D1_POINT_2F(0,cursorSize));
  mouseCursor.emplace_back(D2D1_POINT_2F(0,cursorSizeGap));
  mouseCursor.emplace_back(D2D1_POINT_2F(-cursorSize,0));
  mouseCursor.emplace_back(D2D1_POINT_2F(-cursorSizeGap,0));
  mouseCursor.emplace_back(D2D1_POINT_2F(cursorSize,0));
  mouseCursor.emplace_back(D2D1_POINT_2F(cursorSizeGap,0));

  std::vector<render_line> renderLines;
  renderLines.reserve(4);
  
  CreateDisconnectedRenderLines<D2D1_POINT_2F>(
    mouseCursor.cbegin(), 
    mouseCursor.cend(), 
    std::back_inserter(renderLines), 
    brush, 5, x, y);
  
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}

void RenderDiagnostics(ID2D1RenderTarget* renderTarget, 
                       screen_render_brush_selector brushSelector, 
                       screen_render_text_format_selector textFormatSelector,
                       const std::vector<std::wstring>& diagnosticsData)
{
  for( const auto& text : diagnosticsData )
  {
    RenderText(renderTarget, brushSelector[grey], textFormatSelector[diagnostics], text);
  }
}

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, screen_render_brushes& brushes)
{
  brushes.clear();
  CreateScreenRenderBrushes(renderTarget, std::back_inserter(brushes));
}

void CreateScreenRenderTextFormats(screen_render_text_formats& textFormats)
{
  textFormats.clear();
  CreateScreenRenderTextFormats(std::back_inserter(textFormats));
}

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, std::back_insert_iterator<screen_render_brushes> brushInserter)
{
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_white);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_grey);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_green);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_red);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_yellow);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_cyan);
}

winrt::com_ptr<ID2D1SolidColorBrush> CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color)
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  HRESULT hr = renderTarget->CreateSolidColorBrush(color, brush.put());
  if( FAILED(hr) ) throw(L"error");
  return brush;
}

void CreateScreenRenderTextFormats(std::back_insert_iterator<screen_render_text_formats> textFormatInserter)
{
  winrt::com_ptr<IDWriteFactory> writeFactory;
  HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(writeFactory),reinterpret_cast<IUnknown**>(writeFactory.put()));
  if( FAILED(hr) ) throw L"error";

  textFormatInserter = CreateScreenRenderTextFormat(writeFactory.get(), L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
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

bool ProcessMessage()
{
  MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return (msg.message != WM_QUIT);
	}

  return true;
}
