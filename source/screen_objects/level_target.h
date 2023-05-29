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
  [[nodiscard]] auto Position() const -> game_point;
  auto SetActivated(event_activated eventTargetActivated) -> void;
  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto Geometry() const -> const path_geometry&;
  
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() const -> const collision_data&;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  struct data
  {
    game_point position { 0, 0 };
    bool activated = false;
  };

  std::shared_ptr<data> m_data;
  event_activated m_eventActivated = []() -> void {};

  collision_data m_collisionData;
  collision_effect m_collisionEffect;
  path_geometry m_geometry;
};

#endif
