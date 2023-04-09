#ifndef _level_target_
#define _level_target_

#include "framework.h"
#include "screen_render_data.h"
#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"

struct target_state
{
  target_state(const game_point& position, screen_render_brush_selector brushes);
  
  auto Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

  const game_point& position;
  std::vector<game_point> m_points;
  bool activated = false;
  std::vector<game_line> shape;
  winrt::com_ptr<ID2D1SolidColorBrush> brushNotActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> brushActivated;
  collision_data m_collisionData;
  collision_effect m_collisionEffect;
};

#endif
