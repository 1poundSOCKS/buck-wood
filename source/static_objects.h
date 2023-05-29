#pragma once

#include "active_object_container.h"
#include "level_target.h"
#include "level_asteroid.h"
#include "game_level_object_generator.h"
#include "renderers.h"

class static_objects
{

public:

  using active_object_container_type = active_object_container<collision_data, collision_effect, renderer>;
  using active_object_container_collection = std::list<active_object_container_type>;

  static_objects();

  [[nodiscard]] auto GetObjectContainers() -> active_object_container_collection&;

  [[nodiscard]] auto GetTargets() -> active_object_container_type&;
  [[nodiscard]] auto GetAsteroids() -> active_object_container_type&;

  auto SetCentre(int x, int y) -> void;

  auto Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto DoCollisionsWith(active_object_container_type& objectContainer) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  active_object_container_collection m_objectContainers;

  int m_x;
  int m_y;

};
