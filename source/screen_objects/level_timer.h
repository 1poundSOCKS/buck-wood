#pragma once

#include "object_input_data.h"

class level_timer
{
public:

  using time_getter = std::function<int64_t()>;

  level_timer();
  auto SetTimeGetter(time_getter timeGetter) -> void;

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  time_getter m_timeGetter = []() -> int64_t { return 0; };
};
