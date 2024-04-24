#pragma once

#include "level_cell_collection.h"

class cell_collision_tests
{

public:

  auto operator()(const level_cell_collection& cells, std::ranges::input_range auto&& objectCollection, auto&& callable) -> void;
  auto operator()(const level_cell_collection& cells, auto&& object, auto&& callable) -> void;

private:

  std::mutex m_mutex;

};

auto cell_collision_tests::operator()(const level_cell_collection& cells, std::ranges::input_range auto&& objectCollection, auto&& callable) -> void
{
    std::for_each(std::execution::par, std::begin(objectCollection), std::end(objectCollection), [this,&cells,&callable](auto& object)
    {
      (*this)(cells, object, callable);
    });
}

auto cell_collision_tests::operator()(const level_cell_collection& cells, auto&& object, auto&& callable) -> void
{
    std::lock_guard<std::mutex> guard(m_mutex);

    auto cellType = cells.CellType(object.Object().Position());

    switch( cellType )
    {
      case level_cell_collection::cell_type::wall:
        if( !object.Object().Destroyed() )
        {
          callable(object.Object());
        }
        break;
    }
    
}
