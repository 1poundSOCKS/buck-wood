#include "pch.h"
#include "text_renderer.h"
#include "render_target.h"

text_renderer::text_renderer(const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<IDWriteTextFormat>& textFormat) :
  m_brush { brush }, m_textFormat { textFormat }
{
}

auto text_renderer::Write(const D2D1_RECT_F& rect, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, LPCWSTR text) -> void
{
  if( SUCCEEDED(m_textFormat->SetParagraphAlignment(paragraphAlignment)) && SUCCEEDED(m_textFormat->SetTextAlignment(textAlignment)) )
  {
    render_target::renderText(rect, m_brush.get(), m_textFormat.get(), text);
  }
}

auto text_renderer::Write(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, LPCWSTR text) -> void
{
  if( SUCCEEDED(m_textFormat->SetParagraphAlignment(paragraphAlignment)) && SUCCEEDED(m_textFormat->SetTextAlignment(textAlignment)) )
  {
    render_target::renderText(m_brush.get(), m_textFormat.get(), text);
  }
}
