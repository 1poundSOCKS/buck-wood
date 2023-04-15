#pragma once

#include "screen_render_data.h"

class level_timer
{
public:

  level_timer(screen_render_brush_selector brushes, screen_render_text_format_selector textFormats, int timeInSeconds);
  auto Update(int64_t clockFrequency, int64_t clockCount) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void;
  [[nodiscard]] auto HasExpired() const -> bool;
  [[nodiscard]] auto GetTimeRemainingInSeconds() const -> float;
  auto Stop() -> void;

private:

  struct shared_data_type
  {
    bool stopped = false;
    int levelTime = 0;
    int64_t clockFrequency = 0;
    int64_t timeRemaining = 0;
  };

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  winrt::com_ptr<IDWriteTextFormat> m_textFormat;
  std::shared_ptr<shared_data_type> m_sharedData;
};
