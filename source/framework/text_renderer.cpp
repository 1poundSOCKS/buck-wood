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

auto text_renderer::Write(const D2D1_RECT_F& rect, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, const std::wstring_view& text) const -> void
{
  m_textFormat->SetParagraphAlignment(paragraphAlignment);
  m_textFormat->SetTextAlignment(textAlignment);
  render_target::get()->DrawText(text.data(), static_cast<UINT32>(text.length()), m_textFormat.get(), rect, m_brush.get());
}

auto text_renderer::Write(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, const std::wstring_view& text) const -> void
{
  m_textFormat->SetParagraphAlignment(paragraphAlignment);
  m_textFormat->SetTextAlignment(textAlignment);
  auto renderTargetSize = render_target::get()->GetSize();
  D2D1_RECT_F rect { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1 };
  render_target::get()->DrawText(text.data(), static_cast<UINT32>(text.length()), m_textFormat.get(), rect, m_brush.get());
}

auto text_renderer::Write(const std::wstring_view& text) const -> void
{
  auto renderTargetSize = render_target::get()->GetSize();
  D2D1_RECT_F rect { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1 };
  render_target::get()->DrawText(text.data(), static_cast<UINT32>(text.length()), m_textFormat.get(), rect, m_brush.get());
}
