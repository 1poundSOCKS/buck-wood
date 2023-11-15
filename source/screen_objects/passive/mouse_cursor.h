#pragma once

#include "framework.h"

class mouse_cursor
{
public:

  mouse_cursor();

  auto Update(const screen_input_state& screenInputState) -> void;
  [[nodiscard]] auto Position() const -> std::tuple<float, float>;
  // auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  // auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  
  // winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  float m_x = 0;
  float m_y = 0;
};

[[nodiscard]] inline auto mouse_cursor::Position() const -> std::tuple<float, float>
{
  return { m_x, m_y };
}
