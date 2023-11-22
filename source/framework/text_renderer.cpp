#include "pch.h"
#include "text_renderer.h"
#include "render_target.h"

text_renderer::text_renderer(const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<IDWriteTextFormat>& textFormat) :
  m_brush { brush }, m_textFormat { textFormat }
{
}

text_renderer::text_renderer(const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<IDWriteTextFormat>& textFormat, 
  DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment) : m_brush { brush }, m_textFormat { textFormat }
{
  m_textFormat->SetParagraphAlignment(paragraphAlignment);
  m_textFormat->SetTextAlignment(textAlignment);
}

auto text_renderer::Write(const D2D1_RECT_F& rect, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, const std::wstring_view& text) -> void
{
  m_textFormat->SetParagraphAlignment(paragraphAlignment);
  m_textFormat->SetTextAlignment(textAlignment);
  render_target::renderText(rect, m_brush.get(), m_textFormat.get(), text);
}

auto text_renderer::Write(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, const std::wstring_view& text) -> void
{
  m_textFormat->SetParagraphAlignment(paragraphAlignment);
  m_textFormat->SetTextAlignment(textAlignment);
  render_target::renderText(m_brush.get(), m_textFormat.get(), text);
}

auto text_renderer::Write(const std::wstring_view& text) -> void
{
  render_target::renderText(m_brush.get(), m_textFormat.get(), text);
}
