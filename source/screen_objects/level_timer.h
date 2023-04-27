#pragma once

#include "object_input_data.h"

class level_timer
{
public:

  class control
  {
    friend class level_timer;

  public:
    
    auto GetValue() const -> int64_t;
    auto SetValue(int64_t value) -> void;

  private:

    int64_t m_value = 0;
  };

  using control_data = std::shared_ptr<control>;

  level_timer(int64_t value);
  auto GetControlData() const -> control_data;

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  control_data m_controlData;
};
