#pragma once

#include "level_objects.h"

namespace render_order
{

  inline constexpr auto max_value() -> int
  {
    return 2;
  }

  struct default_object_visitor
  {
    constexpr auto operator()(const player_ship&) -> int
    {
      return 1;
    }

    constexpr auto operator()(const level_target&) -> int
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
