#pragma once

#include "object_input_data.h"

class mouse_cursor
{
public:

  mouse_cursor();

  auto Update(const object_input_data& inputData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  float m_x = 0;
  float m_y = 0;
};
