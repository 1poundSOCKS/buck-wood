#pragma once

#include "object_input_data.h"

class level_timer
{
public:

  using timer_update = std::function<void(int64_t)>;

  level_timer(int64_t value);
  auto SetTimerUpdate(timer_update timerUpdate) -> void;

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  timer_update m_timerUpdate = [](int64_t)->void{};
  int64_t m_value = 0;
};
