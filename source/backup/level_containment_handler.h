#pragma once

#include "default_object.h"

class level_containment_handler
{

public:

  auto operator()(default_object& object1, default_object& object2) -> void;

  [[nodiscard]] auto ExitCell() -> std::optional<POINT_2I>;

private:

  template <typename object_type_1, typename object_type_2> auto OnContainment(default_object& object1, default_object& object2) -> void;
  template <typename object_type> auto OnContainment(default_object& object, const level_cell_item& cell) -> void;

  auto OnContainment(player_ship& player, portal& portalObj) -> void;
  auto OnContainment(auto&& object1, auto&& object2) -> void;

private:

  std::optional<POINT_2I> m_exitCell;

};

template <typename object_type_1, typename object_type_2> auto level_containment_handler::OnContainment(default_object& object1, default_object& object2) -> void
{
  if( std::holds_alternative<object_type_1>(object1.Get()) && std::holds_alternative<object_type_2>(object2.Get()) )
  {
    return OnContainment(std::get<object_type_1>(object1.Get()), std::get<object_type_2>(object2.Get()));
  }

  if( std::holds_alternative<object_type_1>(object2.Get()) && std::holds_alternative<object_type_2>(object1.Get()) )
  {
    return OnContainment(std::get<object_type_1>(object2.Get()), std::get<object_type_2>(object1.Get()));
  }
}

auto level_containment_handler::OnContainment(auto&& object1, auto&& object2) -> void
{
}

inline auto level_containment_handler::ExitCell() -> std::optional<POINT_2I>
{
  return m_exitCell;
}
