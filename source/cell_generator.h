#pragma once

#include "geometry.h"

class cell_generator
{

public:

  struct cell_id
  {
    int column;
    int row;
  };

  using collection = std::vector<cell_id>;

  cell_generator(game_rect rect, float cellWidth, float cellHeight);
  auto Get(std::back_insert_iterator<collection> inserter) const -> void;

private:

  game_rect m_rect;
  float m_cellWidth = 0;
  float m_cellHeight = 0;

};
