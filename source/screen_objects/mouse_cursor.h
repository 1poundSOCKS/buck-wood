#pragma once

#include "screen_render_data.h"

class mouse_cursor
{
public:

  mouse_cursor();

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  auto SetPosition(float x, float y) -> void;

private:

  struct shared_data_type
  {
    float x = 0;
    float y = 0;
  };

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  std::shared_ptr<shared_data_type> m_sharedData;
};
