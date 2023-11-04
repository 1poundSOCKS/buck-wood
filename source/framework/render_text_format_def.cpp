#include "pch.h"
#include "render_text_format_def.h"
#include "screen_render.h"

render_text_format_def::render_text_format_def(const std::wstring& fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize) : 
  m_fontFamily(fontFamily), m_fontWeight(fontWeight), m_fontStyle(fontStyle), m_fontStretch(fontStretch), m_fontSize(fontSize)
{
}

[[nodiscard]] auto render_text_format_def::CreateTextFormat(IDWriteFactory* writeFactory) const -> winrt::com_ptr<IDWriteTextFormat>
{
  return CreateScreenRenderTextFormat(writeFactory, m_fontFamily.c_str(), m_fontWeight, m_fontStyle, m_fontStretch, m_fontSize);
}
