#pragma once

#include "screen_render_data.h"

class mouse_cursor
{
public:

  mouse_cursor(screen_render_brush_selector brushes);

  auto Update(int64_t clockFrequency, int64_t clockCount) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void;

  auto SetPosition(float x, float y) -> void;

private:

  struct shared_data_type
  {
    float x = 0;
    float y = 0;
  };

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  std::shared_ptr<shared_data_type> m_sharedData;
};
