#pragma once

#include "level_cell_collection.h"

class cell_collision_tests
{

public:

  auto operator()(const level_cell_collection& cells, std::ranges::input_range auto&& objectCollection, auto&& callable) -> void;
  auto operator()(const level_cell_collection& cells, auto&& object, auto&& callable) -> void;
  auto particles(const level_cell_collection& cells, std::ranges::input_range auto&& particleCollection, auto&& callable) -> void;

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

    using key_type = std::tuple<int, int>;

    constexpr auto adjacentCellIds = std::array {
      key_type(-1,-1),
      key_type(0,-1),
      key_type(1,-1),
      key_type(-1,0),
      key_type(0,0),
      key_type(1,0),
      key_type(-1,1),
      key_type(0,1),
      key_type(1,1)
    };

    const auto& [objectColumn, objectRow] = cells.CellId(object.Object().Position());
    
    const auto& cellsMap = cells.Get();

    auto adjacentCellIterators = std::ranges::views::transform(adjacentCellIds, [&cellsMap,objectColumn,objectRow](const auto& cellId)
    {
      const auto& [column, row] = cellId;
      level_cell_collection::cell_id id = { column + objectColumn, row + objectRow };
      return cellsMap.find(id);
    });

    auto adjacentWallIterators = std::ranges::views::filter(adjacentCellIterators, [&cellsMap](const auto& cellIterator)
    {
      if( cellIterator == std::end(cellsMap) )
      {
        return false;
      }
      else
      {
        const auto& [key, value] = *cellIterator;
        return value.Type() == level_cell_type::wall;
      }
    });

    for( const auto& cellIterator : adjacentWallIterators )
    {
      const auto& [key, value] = *cellIterator;

      D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
      HRESULT hr = object.Geometry()->CompareWithGeometry(value.Geometry().get(), D2D1::Matrix3x2F::Identity(), &relation);

      bool collided = false;

      if( SUCCEEDED(hr) )
      {
        switch( relation )
        {
          case D2D1_GEOMETRY_RELATION_IS_CONTAINED:
          case D2D1_GEOMETRY_RELATION_CONTAINS:
          case D2D1_GEOMETRY_RELATION_OVERLAP:
            collided = true;
            break;
        }
      }

      if( collided )
      {
        if( !object.Object().Destroyed() )
        {
          callable(object.Object());
        }
      }
    }

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

auto cell_collision_tests::particles(const level_cell_collection& cells, std::ranges::input_range auto&& particleCollection, auto&& callable) -> void
{
  std::for_each(std::execution::par, std::begin(particleCollection), std::end(particleCollection), [this,&cells,&callable](auto& particle)
  {
    std::lock_guard<std::mutex> guard(m_mutex);

    auto cellType = cells.CellType(particle.Position());

    switch( cellType )
    {
      case level_cell_collection::cell_type::wall:
        if( !particle.Destroyed() )
        {
          callable(particle);
        }
        break;
    }
  });
}
