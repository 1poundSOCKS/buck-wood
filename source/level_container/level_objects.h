#pragma once

#include "objects/player_ship.h"
#include "objects/player_bullet.h"
#include "objects/enemy_ship.h"
#include "objects/enemy_bullet_1.h"
#include "objects/portal.h"
#include "objects/power_up.h"
#include "objects/blank_object.h"
#include "objects/background_object.h"
#include "objects/level_cell.h"
#include "objects/particle.h"
#include "objects/boundary_walls.h"
#include "objects/boundary_exit.h"

enum class object_type {
  player, 
  player_bullet, 
  enemy_1, 
  enemy_bullet_1, 
  portal, 
  power_up, 
  cell, 
  boundary_walls, 
  boundary_exit_left,
  boundary_exit_top,
  boundary_exit_right,
  boundary_exit_bottom
};

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

  inline auto Type(const enemy_ship&) -> object_type
  {
    return object_type::enemy_1;
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

  inline auto Type(const boundary_exit& object) -> object_type
  {
    switch( object.Type() )
    {
      case boundary_exit::type::left:
        return object_type::boundary_exit_left;
      case boundary_exit::type::top:
        return object_type::boundary_exit_top;
      case boundary_exit::type::right:
        return object_type::boundary_exit_right;
      case boundary_exit::type::bottom:
        return object_type::boundary_exit_bottom;
      default:
        return object_type::boundary_exit_left;
    }
  }

}
