#include "pch.h"
#include "render.h"

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

render_point::render_point(float x, float y, ID2D1SolidColorBrush* brush, float size)
: rect({x - size, y - size, x + size, y + size}), brush(brush)
{
}

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, render_brushes::color brushColor)
: start(start), end(end), brushColor(brushColor)
{
}

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width)
: start(start), end(end), brush(brush), width(width)
{
}

void RenderText(
  ID2D1RenderTarget* renderTarget, 
  ID2D1SolidColorBrush* brush, 
  IDWriteTextFormat* textFormat, 
  std::wstring_view text,
  DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, 
  DWRITE_TEXT_ALIGNMENT textAlignment)
{
  HRESULT hr = textFormat->SetParagraphAlignment(paragraphAlignment);
  if( FAILED(hr) ) throw L"error";

  hr = textFormat->SetTextAlignment(textAlignment);
  if( FAILED(hr) ) throw L"error";

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
