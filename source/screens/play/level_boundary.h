#ifndef _level_boundary_
#define _level_boundary_

#include "geometry.h"
#include "game_level_data.h"
#include "screen_render_data.h"

struct level_boundary
{
  level_boundary(const game_open_object& object, screen_render_brush_selector brushes);
  
  auto Update(int64_t tickFrequency, int64_t tickCount) -> void;
  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  void RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const;

  game_open_object object;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
};

[[nodiscard]] auto LoadLevelBoundary(const game_level_data& levelData) -> game_open_object;

#endif
