#pragma once

class text_renderer
{

public:

  text_renderer(const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<IDWriteTextFormat>& textFormat);
  text_renderer(const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<IDWriteTextFormat>& textFormat, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment);

  auto Write(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, const std::wstring_view& text) const -> void;
  auto Write(const D2D1_RECT_F& rect, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, DWRITE_TEXT_ALIGNMENT textAlignment, const std::wstring_view& text) const -> void;
  auto Write(const std::wstring_view& text) const -> void;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;

};
