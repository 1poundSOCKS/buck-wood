#include "pch.h"
#include "render.h"

void RenderLines(
  const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
  const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
  std::vector<render_line>::const_iterator begin, 
  std::vector<render_line>::const_iterator end);

void RenderLine(
  const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
  const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
  const render_line& line, 
  float renderWidth);

render_point::render_point(float x, float y, ID2D1SolidColorBrush* brush, float size)
: rect({x - size, y - size, x + size, y + size}), brush(brush)
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

void RenderLines(
  const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
  const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
  std::vector<render_line>::const_iterator begin, 
  std::vector<render_line>::const_iterator end)
{
  for( std::vector<render_line>::const_iterator line = begin; line != end; line++ )
  {
    renderTarget->DrawLine(line->start, line->end, brush.get(), 2.0f);
  }
}

void RenderLines(
  ID2D1RenderTarget* renderTarget, 
  std::vector<render_line>::const_iterator begin, 
  std::vector<render_line>::const_iterator end)
{
  for( std::vector<render_line>::const_iterator line = begin; line != end; line++ )
  {
    renderTarget->DrawLine(line->start, line->end, line->brush, line->width);
  }
}

void RenderLine(
  const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
  const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
  const render_line& line, 
  float renderWidth)
{
  renderTarget->DrawLine(line.start, line.end, brush.get(), renderWidth);
}
