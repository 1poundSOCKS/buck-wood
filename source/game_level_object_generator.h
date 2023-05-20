#pragma once

#include "geometry.h"
// #include "level_grid_cell_generator.h"
#include "level_target.h"
// #include "rect_generator.h"
#include "active_object_container.h"

class game_level_object_generator
{

public:

  using star_collection = std::vector<game_point>;
  using asteroid_collection = std::vector<game_closed_object>;
  using target_collection = std::vector<level_target>;
  using asteroid_inserter = active_object_container<collision_data, collision_effect>::inserter;

  game_level_object_generator(int minColumn, int maxColumn, int columnWidth, int minRow, int maxRow, int rowHeight, float noiseLower, float noiseUpper, float noiseDial);
  auto InsertInto(std::back_insert_iterator<star_collection> starInserter) const -> void;
  auto InsertInto(std::back_insert_iterator<asteroid_collection> asteroidInserter) const -> void;
  auto InsertInto(std::back_insert_iterator<target_collection> targetInserter) const -> void;
  auto InsertAsteroidsInto(asteroid_inserter inserter) const -> void;

private:

  [[nodiscard]] auto CreateAsteroid(game_rect rect) const -> game_closed_object;
  auto CreateAsteroid(game_rect rect, asteroid_inserter inserter) const -> void;
  [[nodiscard]] auto CreateTarget(game_rect rect) const -> game_closed_object;

  int m_minColumn = 0;
  int m_maxColumn = 0;
  int m_columnWidth = 0;
  int m_minRow = 0;
  int m_maxRow = 0; 
  int m_rowHeight = 0;
  float m_noiseLower = -1.0f;
  float m_noiseUpper = 1.0f;
  float m_noiseDial = 1.0f;
};
