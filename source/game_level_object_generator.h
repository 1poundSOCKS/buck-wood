#pragma once

#include "geometry.h"
#include "level_grid_cell_generator.h"
#include "level_target.h"
#include "rect_generator.h"

class game_level_object_generator
{

public:

  using star_collection = std::list<game_point>;
  using asteroid_collection = std::list<game_closed_object>;
  using target_collection = std::list<level_target>;

  game_level_object_generator(int minColumn, int maxColumn, int columnWidth, int minRow, int maxRow, int rowHeight);
  auto InsertInto(std::back_insert_iterator<star_collection> starInserter) const -> void;
  auto InsertInto(std::back_insert_iterator<asteroid_collection> asteroidInserter, std::back_insert_iterator<target_collection> targetInserter) const -> void;
  auto InsertInto(std::back_insert_iterator<target_collection> inserter) const -> void;

private:

  [[nodiscard]] auto CreateAsteroid(game_rect rect) const -> game_closed_object;
  [[nodiscard]] auto CreateTarget(game_rect rect) const -> game_closed_object;

  int m_minColumn = 0;
  int m_maxColumn = 0;
  int m_columnWidth = 0;
  int m_minRow = 0;
  int m_maxRow = 0; 
  int m_rowHeight = 0;
};
