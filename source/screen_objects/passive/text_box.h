#pragma once

#include "object_input_data.h"
#include "render_target_area.h"

class text_box
{
public:

  using text_getter = std::function<std::wstring()>;
  using callback_for_flag = std::function<bool()>;

  text_box(render_target_area renderTargetArea);
  auto SetTextGetter(text_getter textGetter) -> void;
  auto SetCallbackForHiddenFlag(callback_for_flag callbackForHiddenFlag) -> void;

  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;

  render_target_area m_renderTargetArea;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;

  static constexpr std::wstring m_defaultText = std::wstring(L"");
  std::wstring m_textValue = m_defaultText;
  text_getter m_textGetter = []() -> std::wstring { return m_defaultText; };

  bool m_hidden = false;
  callback_for_flag m_callbackForHiddenFlag = [](){ return false; };
};
