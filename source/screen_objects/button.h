#pragma once

#include "object_input_data.h"
#include "render_text_format_def.h"

class button
{
public:

  button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> m_eventClicked);

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  [[nodiscard]] static auto IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool;

  std::wstring m_text = L"";
  float m_buttonHeight = 0;
  D2D1_RECT_F m_rect = { 0, 0, 0, 0 };

  render_text_format_def m_textFormatDef;
  render_text_format_def m_hoverTextFormatDef;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonBorderBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonHoverBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_textBrush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  winrt::com_ptr<IDWriteTextFormat> m_hoverTextFormat;

  std::function<void()> m_eventClicked = [](){};

  bool m_hover = false;
};
