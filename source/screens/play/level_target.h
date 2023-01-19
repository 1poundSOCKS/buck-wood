#ifndef _level_target_
#define _level_target_

#include "framework.h"
#include "screen_render_data.h"

struct target_state
{
  target_state(const game_point& position, screen_render_brush_selector brushes);

  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
  std::vector<game_line> shape;
  winrt::com_ptr<ID2D1SolidColorBrush> brushNotActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> brushActivated;
};

[[nodiscard]] auto HasCollided(const target_state& target, float x, float y) -> bool;
auto HitByBullet(target_state& targetState) -> void;
auto GetRenderLines(const target_state& targetState, render_line_inserter_type inserter) -> void;
auto LevelIsComplete(const target_state& targetState) -> bool;

#endif
