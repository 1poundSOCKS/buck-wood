#pragma once

#include "level_objects.h"

namespace render_order
{

  inline constexpr auto end() -> int
  {
    return 4;
  }

  inline constexpr auto particle_end() -> int
  {
    return 2;
  }

  struct default_object_visitor
  {
    constexpr auto operator()(const player_ship&) -> int
    {
      return 3;
    }

    constexpr auto operator()(const enemy_type_2&) -> int
    {
      return 3;
    }

    constexpr auto operator()(const enemy_type_3&) -> int
    {
      return 3;
    }

    constexpr auto operator()(const player_bullet&) -> int
    {
      return 2;
    }

    constexpr auto operator()(const enemy_bullet_1&) -> int
    {
      return 2;
    }

    constexpr auto operator()(const portal&) -> int
    {
      return 1;
    }

    constexpr auto operator()(auto&& object) -> int
    {
      return 0;
    }
  };

  inline auto get(const default_object& object) -> int
  {
    static default_object_visitor visitor {};
    return std::visit(visitor, object.Get());
  }

}
