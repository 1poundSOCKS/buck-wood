#ifndef _level_island_
#define _level_island_

#include "framework.h"
#include "screen_render_data.h"
#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"

struct level_island
{
  level_island(const game_closed_object& object, screen_render_brush_selector brushes);

  auto Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

  game_closed_object object;
  collision_effect m_collisionEffect;
  collision_data m_collisionData;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
};

#endif