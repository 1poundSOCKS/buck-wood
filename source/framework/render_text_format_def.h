#pragma once

class render_text_format_def
{
public:

  render_text_format_def(const std::wstring& fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, 
    DWRITE_FONT_STRETCH m_fontStretch, float fontSize);

  [[nodiscard]] auto CreateTextFormat(IDWriteFactory* writeFactory) const -> winrt::com_ptr<IDWriteTextFormat>;

private:
  std::wstring m_fontFamily;
  DWRITE_FONT_WEIGHT m_fontWeight;
  DWRITE_FONT_STYLE m_fontStyle;
  DWRITE_FONT_STRETCH m_fontStretch;
  float m_fontSize;
};
