#pragma once

#include "level_container.h"
#include "level_object_movement.h"
#include "level_data.h"

class game_world
{

public:

  game_world();

  auto LoadLevel(int levelIndex) const -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container& levelContainer) const noexcept -> void;

};
