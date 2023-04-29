#ifndef _level_target_
#define _level_target_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"

class level_target
{
public:

  using event_activated = std::function<void()>;

  level_target(float x, float y, event_activated eventTargetActivated);
  
  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  std::vector<game_point> m_points;
  std::vector<game_line> m_shape;
  bool m_activated = false;
  event_activated m_eventTargetActivated;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushNotActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushActivated;
  collision_data m_collisionData;
  collision_effect m_collisionEffect;
};

#endif
