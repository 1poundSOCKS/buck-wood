#pragma once

#include "geometry.h"
#include "level_asteroid.h"
#include "level_grid.h"

class asteroid_container
{
public:

  using asteroid_collection = std::list<level_asteroid>;

  asteroid_container() = default;

  auto Update(const D2D1_RECT_F& rect) -> void;

  auto begin() const -> asteroid_collection::const_iterator;
  auto end() const -> asteroid_collection::const_iterator;

private:

  auto CreateSmallAsteroids(const level_grid& grid, auto inserter) -> void;
  auto CreateLargeAsteroids(const level_grid& grid, auto inserter) -> void;

  level_grid m_smallGrid;
  level_grid m_largeGrid;

  asteroid_collection m_asteroids;
  
};
