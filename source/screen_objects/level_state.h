#pragma once

#include "object_input_data.h"

class level_state
{
public:

  class control  
  {
    friend class level_state;

  public:
  
    enum state_type { playing, paused, dead, complete };
    auto SetState(state_type stateValue) -> void;
    auto GetText() const -> LPCWSTR;

  private:

    state_type m_state = playing;
  };

  using control_data = std::shared_ptr<control>;

  level_state();
  auto GetControlData() const -> control_data;

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  control_data m_controlData;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
};
