#pragma once

class level_timer
{
public:

  class control
  {
    friend class level_timer;

  public:
    
    auto SetValue(float value) -> void;

  private:

    float m_value = 0;
  };

  using control_data = std::shared_ptr<control>;

  level_timer(control_data controlData);

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  control_data m_controlData;
};
