#pragma once

#include "geometry.h"
#include "cell_generator.h"

class rect_generator
{

public:

  using collection = std::vector<game_rect>;

  rect_generator(game_rect rect, int columnCount, int rowCount);
  auto Get(std::back_insert_iterator<collection> inserter, std::function<bool(float)> noiseValueCheck) const -> void;

private:

  auto GetRect(cell_generator::cell_id cellID) const -> game_rect;

  game_rect m_rect = { { 0, 0} , { 0, 0 } };
  float m_cellWidth = 0;
  float m_cellHeight = 0;
  cell_generator m_cellGenerator;

};
