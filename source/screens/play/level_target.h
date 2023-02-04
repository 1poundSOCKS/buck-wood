#ifndef _level_target_
#define _level_target_

#include "framework.h"
#include "screen_render_data.h"

struct target_state
{
  target_state(const game_point& position, screen_render_brush_selector brushes);
  
  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  void RenderTo(ID2D1RenderTarget* renderTarget) const;

  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
  std::vector<game_line> shape;
  winrt::com_ptr<ID2D1SolidColorBrush> brushNotActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> brushActivated;
};

#endif
