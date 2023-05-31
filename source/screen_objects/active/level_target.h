#ifndef _level_target_
#define _level_target_

#include "play_event.h"
#include "object_input_data.h"
#include "path_geometry.h"

class level_target
{
public:

  level_target(float x, float y);

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto Geometry() const -> const path_geometry&;
  auto Activate() -> void;
  
private:

  game_point m_position { 0, 0 };
  bool m_activated = false;
  path_geometry m_geometry;

};

#endif
