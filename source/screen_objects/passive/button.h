#pragma once

#include "object_input_data.h"
#include "render_text_format_def.h"

class button
{
public:

  using callback_for_flag = std::function<bool()>;

  button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> m_eventClicked);

  auto SetCallbackForHiddenFlag(callback_for_flag callbackForHiddenFlag) -> void;
  auto SetCallbackForEnabledFlag(callback_for_flag callbackForEnabledFlag) -> void;
  auto GetHoverState() const -> bool;

  auto Update(const object_input_data& inputData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  [[nodiscard]] static auto IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool;

  std::wstring m_text = L"";
  float m_buttonHeight = 0;
  D2D1_RECT_F m_rect = { 0, 0, 0, 0 };
  std::function<void()> m_eventClicked = [](){};

  bool m_hidden { false };
  callback_for_flag m_callbackForHiddenFlag = [](){ return false; };

  bool m_enabled { true };
  callback_for_flag m_callbackForEnabledFlag = [](){ return true; };  

  render_text_format_def m_textFormatDef;
  render_text_format_def m_hoverTextFormatDef;

  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonBorderBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonHoverBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_textBrush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  winrt::com_ptr<IDWriteTextFormat> m_hoverTextFormat;

  bool m_hover { false };
};
