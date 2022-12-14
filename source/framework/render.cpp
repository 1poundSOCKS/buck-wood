#include "pch.h"
#include "render.h"

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
  renderTarget->DrawText(text.data(), static_cast<UINT32>(text.length()), textFormat, rect, brush);
}
