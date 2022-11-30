#include "pch.h"
#include "render.h"

void RenderText(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<IDWriteTextFormat>& textFormat, 
                const winrt::com_ptr<ID2D1SolidColorBrush>& brush, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment,
                D2D1_RECT_F rect, const std::wstring_view& text);
void RenderLines(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end);
void RenderLine(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const render_line& line, float renderWidth);

render_brushes::render_brushes(ID2D1RenderTarget* renderTarget)
{
  this->renderTarget.attach(renderTarget);
  this->renderTarget->AddRef();

  HRESULT hr = S_OK;
  
  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f)), brushWhite.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f)), brushGrey.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f)), brushGreen.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f)), brushRed.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f)), brushYellow.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f)), brushCyan.put());
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

render_text_formats::render_text_formats()
{
  HRESULT hr = S_OK;
  winrt::com_ptr<IDWriteFactory> writeFactory;

  hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(writeFactory),reinterpret_cast<IUnknown**>(writeFactory.put()));
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Verdana",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,20,L"", writeTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Verdana",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,40,L"", menuTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Aldhabi",NULL,DWRITE_FONT_WEIGHT_BOLD,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,60,L"", levelTimerTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Aldhabi",NULL,DWRITE_FONT_WEIGHT_BOLD,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,100,L"", levelEndTextFormat.put());
  if( FAILED(hr) ) throw L"error";
}

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, render_brushes::color brushColor)
: start(start), end(end), brushColor(brushColor)
{
}

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width)
: brush(brush)
{
}

void RenderDiagnostics(const render_brushes& brushes, const render_text_formats& textFormats, const std::vector<std::wstring>& diagnosticsData)
{
  D2D_SIZE_F size = brushes.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);

  std::wstring msg;
  for( const auto& text: diagnosticsData )
  {
    msg += text;
    msg += L"\n";
  }

  RenderText(brushes.renderTarget, textFormats.writeTextFormat, brushes.brushGrey, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_LEADING, rect, msg);
}

void RenderTimer(const render_brushes& brushes, const render_text_formats& textFormats, float seconds)
{
  D2D_SIZE_F size = brushes.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(size.width * 7 / 8, size.height / 16, size.width - 1, size.height * 3 / 16);

  static wchar_t timeText[64];
  swprintf(timeText, L"%.2f", seconds);

  RenderText(brushes.renderTarget, textFormats.levelTimerTextFormat, brushes.brushYellow, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING, rect, timeText);
}

void RenderMainScreenPrompt(const render_brushes& brushes, const render_text_formats& textFormats, const std::wstring_view& text)
{
  D2D_SIZE_F size = brushes.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  brushes.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  RenderText(brushes.renderTarget, textFormats.menuTextFormat, brushes.brushCyan, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, rect, text);
}

void RenderPlayStatus(const render_brushes& brushes, const render_text_formats& textFormats, const std::wstring_view& text)
{
  D2D_SIZE_F size = brushes.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  brushes.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  RenderText(brushes.renderTarget, textFormats.levelEndTextFormat, brushes.brushCyan, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, rect, text);
}

void RenderText(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget,
                const winrt::com_ptr<IDWriteTextFormat>& textFormat,
                const winrt::com_ptr<ID2D1SolidColorBrush>& brush,
                DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment,
                DWRITE_TEXT_ALIGNMENT textAlignment,
                D2D1_RECT_F rect,
                const std::wstring_view& text)
{
  HRESULT hr;

  hr = textFormat->SetParagraphAlignment(paragraphAlignment);
  if( FAILED(hr) ) throw L"error";

  hr = textFormat->SetTextAlignment(textAlignment);
  if( FAILED(hr) ) throw L"error";

  renderTarget->DrawTextW(text.data(),text.length(), textFormat.get(), rect, brush.get());
}

void RenderText(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, std::wstring_view text)
{
  D2D1_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect { 0, 0, size.width - 1, size.height - 1 };
  renderTarget->DrawText(text.data(), text.length(), textFormat, rect, brush);
}

void RenderPoint(const render_brushes& brushes, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_point& point)
{
  brushes.renderTarget->FillRectangle(&point.rect, GetBrush(brushes, point.brushColor).get());
}

void RenderLines(const render_brushes& brushes, float renderWidth, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end)
{
  for( auto line = begin; line != end; ++line )
  {
    RenderLine(brushes.renderTarget, GetBrush(brushes, line->brushColor), *line, renderWidth);
  }
}

void RenderLines(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end)
{
  for( std::vector<render_line>::const_iterator line = begin; line != end; line++ )
  {
    renderTarget->DrawLine(line->start, line->end, brush.get(), 2.0f);
  }
}

void RenderLines(ID2D1RenderTarget* renderTarget, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end)
{
  for( std::vector<render_line>::const_iterator line = begin; line != end; line++ )
  {
    renderTarget->DrawLine(line->start, line->end, line->brush, line->width);
  }
}

void RenderLine(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const render_line& line, float renderWidth)
{
  renderTarget->DrawLine(line.start, line.end, brush.get(), renderWidth);
}

void RenderPoints(const render_brushes& brushes, std::vector<render_point>::const_iterator begin, std::vector<render_point>::const_iterator end)
{
  for( std::vector<render_point>::const_iterator point = begin; point != end; ++point )
  {
    brushes.renderTarget->FillRectangle(&point->rect, GetBrush(brushes, point->brushColor).get());
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
