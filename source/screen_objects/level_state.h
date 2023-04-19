#pragma once

class level_state
{
public:

  enum state_type { playing, paused, dead, complete };
  
  level_state();

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  auto SetState(state_type state) -> void;
  auto GetState() const -> state_type;

private:

  struct shared_data_type
  {
    state_type state = playing;
  };

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  std::shared_ptr<shared_data_type> m_sharedData;
};
