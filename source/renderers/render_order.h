#pragma once

#include "default_object.h"

namespace render_order
{

  using value_type = int;

  inline constexpr auto begin() -> value_type
  {
    return 0;
  }

  inline constexpr auto end() -> value_type
  {
    return 4;
  }

  struct default_object_visitor
  {
    constexpr auto operator()(const player_ship&) -> value_type
    {
      return 3;
    }

    constexpr auto operator()(const enemy_ship&) -> value_type
    {
      return 3;
    }

    constexpr auto operator()(const player_bullet&) -> value_type
    {
      return 2;
    }

    constexpr auto operator()(const enemy_bullet&) -> value_type
    {
      return 2;
    }

    constexpr auto operator()(const power_up&) -> value_type
    {
      return 2;
    }

    constexpr auto operator()(const boundary_walls&) -> value_type
    {
      return 0;
    }

    constexpr auto operator()(auto&& object) -> value_type
    {
      return 3;
    }
  };

  inline auto get(const default_object& object) -> value_type
  {
    return object.Visit(default_object_visitor {});
  }

}
