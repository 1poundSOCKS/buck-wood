#ifndef _level_island_
#define _level_island_

#include "framework.h"
#include "screen_render_data.h"
#include "play_event.h"
#include "object_outline.h"

struct level_island
{
  level_island(const game_closed_object& object, screen_render_brush_selector brushes);

  auto Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  void RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const;
  auto GetOutline() -> object_outline;

  game_closed_object object;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  object_outline outline;
};

#endif
