#include "pch.h"
#include "game_world.h"
#include "level_1.h"
#include "level_2.h"

auto game_world::LevelIndex(int index, POINT_2I exitCell) const -> int
{
  return 1;
}

auto game_world::Level(int index) const -> std::unique_ptr<level_base>
{
  switch( index )
  {
    case 0:
      return std::make_unique<level_1>();
      
    case 1:
      return std::make_unique<level_2>();

    default:
      return std::make_unique<level_1>();
      
  }
}
