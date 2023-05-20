#pragma once

#include "active_object_container.h"
#include "level_target.h"
#include "level_asteroid.h"

class static_objects
{

public:

  using active_object_container_type = active_object_container<collision_data, collision_effect>;
  using active_object_container_collection = std::list<active_object_container_type>;

  static_objects();

  auto GetObjectContainers() -> active_object_container_collection&;

  auto AddTarget(level_target target) -> void;
  auto AddAsteroid(level_asteroid asteroid) -> void;

  auto SetCentre(float x, float y) -> void;

  auto Initialize() -> void;
  auto Update(const object_input_data& inputData, int64_t ticks) -> void;
  auto DoCollisionsWith(active_object_container_type& objectContainer) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  active_object_container_collection m_objectContainers;

  float m_x;
  float m_y;

};
