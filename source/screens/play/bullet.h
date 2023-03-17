#ifndef _bullet_
#define _bullet_

#include "screen_render_data.h"
#include "play_event.h"
#include "object_outline.h"

struct bullet
{
  bullet(screen_render_brush_selector brushes);

  auto Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetOutline() -> object_outline;

  float startX = 0, startY = 0;
  float xPos = 0, yPos = 0;
  float xVelocity = 0, yVelocity = 0;
  float angle = 0;
  float range = 1000;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  object_outline outline;
};

#endif
