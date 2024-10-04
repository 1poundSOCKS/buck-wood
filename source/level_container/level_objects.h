#pragma once

#include "objects/player_ship.h"
#include "objects/player_bullet.h"
#include "objects/enemy_type_1.h"
#include "objects/enemy_type_2.h"
#include "objects/enemy_type_3.h"
#include "objects/enemy_bullet_1.h"
#include "objects/portal.h"
#include "objects/power_up.h"
#include "objects/blank_object.h"
#include "objects/background_object.h"
#include "objects/level_cell.h"
#include "objects/particle.h"
#include "objects/boundary_walls.h"

enum class object_type { player, player_bullet, enemy_1, enemy_2, enemy_3, enemy_bullet_1, portal, power_up, cell, boundary_walls };

namespace level_objects
{

  inline auto Type(const player_ship&) -> object_type
  {
    return object_type::player;
  }

  inline auto Type(const player_bullet&) -> object_type
  {
    return object_type::player_bullet;
  }

  inline auto Type(const enemy_type_1&) -> object_type
  {
    return object_type::enemy_1;
  }

  inline auto Type(const enemy_type_2&) -> object_type
  {
    return object_type::enemy_2;
  }

  inline auto Type(const enemy_type_3&) -> object_type
  {
    return object_type::enemy_3;
  }

  inline auto Type(const enemy_bullet_1&) -> object_type
  {
    return object_type::enemy_bullet_1;
  }

  inline auto Type(const portal&) -> object_type
  {
    return object_type::portal;
  }

  inline auto Type(const power_up&) -> object_type
  {
    return object_type::power_up;
  }

  inline auto Type(const level_cell&) -> object_type
  {
    return object_type::cell;
  }

  inline auto Type(const boundary_walls&) -> object_type
  {
    return object_type::boundary_walls;
  }

}
