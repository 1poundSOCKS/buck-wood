#pragma once

#include "screen_render_data.h"

class level_state
{
public:

  enum state_type { playing, paused, dead, complete };
  
  level_state(screen_render_brush_selector brushes, screen_render_text_format_selector textFormats);

  auto Update(int64_t clockFrequency, int64_t clockCount) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void;

  auto SetState(state_type state) -> void;
  auto GetState() const -> state_type;

private:

  struct shared_data_type
  {
    state_type state = playing;
  };

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  std::shared_ptr<shared_data_type> m_sharedData;
};
