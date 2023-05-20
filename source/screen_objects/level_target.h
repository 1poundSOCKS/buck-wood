#ifndef _level_target_
#define _level_target_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"
#include "path_geometry.h"

class level_target
{
public:

  using event_activated = std::function<void()>;

  level_target(float x, float y);
  level_target(const game_closed_object& object);
  auto SetActivated(event_activated eventTargetActivated) -> void;
  
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() const -> const collision_data&;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;

  bool m_activated = false;
  event_activated m_eventActivated = []() -> void {};
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushNotActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushCentre;
  collision_data m_collisionData;
  collision_effect m_collisionEffect;
  path_geometry m_geometry;
  
};

#endif
