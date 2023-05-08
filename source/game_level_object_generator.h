#pragma once

#include "geometry.h"
#include "level_grid_cell_generator.h"
#include "level_target.h"
#include "rect_generator.h"

class game_level_object_generator
{

public:

  using asteroid_collection = std::list<game_closed_object>;
  using target_collection = std::list<level_target>;

  game_level_object_generator(game_rect rect, int columnCount, int rowCount, std::function<bool(float)> generateObject);
  auto InsertInto(std::back_insert_iterator<asteroid_collection> inserter) const -> void;
  auto InsertInto(std::back_insert_iterator<target_collection> inserter) const -> void;

private:

  [[nodiscard]] auto CreateAsteroid(game_rect rect) const -> game_closed_object;

  rect_generator m_rectGenerator;
  std::function<bool(float)> m_generateObject;

};
