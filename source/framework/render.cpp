#include "pch.h"
#include "render.h"

render_brushes::render_brushes(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget)
{
  HRESULT hr = S_OK;
  
  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f)), brushWhite.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f)), brushGreen.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f)), brushRed.put());
  if( FAILED(hr) ) throw(L"error");
}

render_point::render_point(float x, float y, float size, render_brushes::color brushColor)
: brushColor(brushColor)
{
  float halfSize = size / 2;
  rect.left = x - halfSize;
  rect.right = x + halfSize;
  rect.top = y - halfSize;
  rect.bottom = y + halfSize;
}

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, render_brushes::color brushColor)
: start(start), end(end), brushColor(brushColor)
{
}

void CreateRenderBrushes(render_brushes& brushes)
{

}

void RenderDiagnostics(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const std::vector<std::wstring>& diagnosticsData, const dwrite_text_formats& textFormats, const d2d_brushes& brushes)
{
  D2D_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);

  rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  std::wstring msg;
  for( const auto& text: diagnosticsData )
  {
    msg += text;
    msg += L"\n";
  }

  renderTarget->DrawTextW(msg.c_str(),msg.length(), textFormats.writeTextFormat.get(), rect, brushes.brushDiagnostics.get());
}

void RenderTimer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float seconds, const dwrite_text_formats& textFormats, const d2d_brushes& brushes)
{
  D2D_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(size.width * 7 / 8, size.height / 16, size.width - 1, size.height * 3 / 16);

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  static wchar_t timeText[64];
  swprintf(timeText, L"%.2f", seconds);
  renderTarget->DrawTextW(timeText,wcslen(timeText), textFormats.levelTimerTextFormat.get(), rect, brushes.brushTimer.get());
}

void RenderMainScreenPrompt(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
                            const winrt::com_ptr<IDWriteTextFormat>& textFormat, 
                            const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
                            const std::wstring& text)
{
  D2D_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  renderTarget->DrawTextW(text.c_str(),text.length(), textFormat.get(), rect, brush.get());
}

void RenderMouseCursor(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float x, float y, const render_brushes& brushes)
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  std::vector<D2D1_POINT_2F> mouseCursor;
  mouseCursor.reserve(8);
  mouseCursor.push_back(D2D1_POINT_2F(0,-cursorSize));
  mouseCursor.push_back(D2D1_POINT_2F(0,-cursorSizeGap));
  mouseCursor.push_back(D2D1_POINT_2F(0,cursorSize));
  mouseCursor.push_back(D2D1_POINT_2F(0,cursorSizeGap));
  mouseCursor.push_back(D2D1_POINT_2F(-cursorSize,0));
  mouseCursor.push_back(D2D1_POINT_2F(-cursorSizeGap,0));
  mouseCursor.push_back(D2D1_POINT_2F(cursorSize,0));
  mouseCursor.push_back(D2D1_POINT_2F(cursorSizeGap,0));

  std::vector<render_line> renderLines;
  renderLines.reserve(4);
  CreateDisconnectedRenderLines<D2D1_POINT_2F>(mouseCursor.cbegin(), mouseCursor.cend(), std::back_inserter(renderLines), render_brushes::color::color_white, x, y);
  RenderLines(renderTarget, brushes, 5, renderLines.cbegin(), renderLines.cend());
}

void RenderPoint(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_point& point, const render_brushes& brushes)
{
  renderTarget->FillRectangle(&point.rect, GetBrush(brushes, point.brushColor).get());
}

void RenderLines(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_brushes& brushes, float renderWidth, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end)
{
  for( auto line = begin; line != end; ++line )
  {
    RenderLine(renderTarget, *line, renderWidth, GetBrush(brushes, line->brushColor));
  }
}

void RenderLine(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_line& line, float renderWidth, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  renderTarget->DrawLine(line.start, line.end, brush.get(), renderWidth);
}

void RenderLines(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end)
{
  for( std::vector<render_line>::const_iterator line = begin; line != end; line++ )
  {
    renderTarget->DrawLine(line->start, line->end, brush.get(), 2.0f);
  }
}

void RenderPoints(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_brushes& brushes, std::vector<render_point>::const_iterator begin, std::vector<render_point>::const_iterator end)
{
  for( std::vector<render_point>::const_iterator point = begin; point != end; ++point )
  {
    renderTarget->FillRectangle(&point->rect, GetBrush(brushes, point->brushColor).get());
  }
}

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight)
{
  return D2D1::Matrix3x2F::Translation(-centerPosX, -centerPosY) * 
    D2D1::Matrix3x2F::Scale(scale, scale) *
    D2D1::Matrix3x2F::Translation(renderTargetWidth / 2, renderTargetHeight / 2);
}

const winrt::com_ptr<ID2D1SolidColorBrush>& GetBrush(const render_brushes& brushes, render_brushes::color brushColor)
{
  switch( brushColor )
  {
    case render_brushes::color::color_white:
      return brushes.brushWhite;
    case render_brushes::color::color_green:
      return brushes.brushGreen;
    case render_brushes::color::color_red:
      return brushes.brushRed;
    default:
      return brushes.brushWhite;
  }
}
