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

    auto walls = std::ranges::views::filter(cells.Get(), [](const auto& cellEntry)
    {
      const auto& [key, value] = cellEntry;
      return value.Type() == valid_cell::cell_type::wall;
    });

    // auto wallGeometries = std::ranges::views::transform(walls, [](const auto& cellEntry)
    // {
    //   const auto& [key, value] = cellEntry;
    //   return value.Geometry();
    // });

    for( const auto& cellEntry : walls )
    {
      const auto& [key, value] = cellEntry;

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
