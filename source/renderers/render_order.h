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

  inline constexpr auto particle_end() -> value_type
  {
    return 2;
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

    constexpr auto operator()(const portal&) -> value_type
    {
      return 1;
    }

    constexpr auto operator()(const boundary_walls&) -> value_type
    {
      return 0;
    }

    constexpr auto operator()(auto&& object) -> value_type
    {
      return 0;
    }
  };

  inline auto get(const default_object& object) -> value_type
  {
    static default_object_visitor visitor {};
    return object.Visit(visitor);
  }

}
