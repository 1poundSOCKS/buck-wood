#pragma once

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

  level_state(control_data controlData);

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  control_data m_controlData;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
};
