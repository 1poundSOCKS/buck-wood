#pragma once

#include "geometry.h"
#include "level_grid_cell_generator.h"
#include "level_target.h"

class game_level_object_generator
{

public:

  using asteroid_collection = std::list<game_closed_object>;
  using target_collection = std::list<level_target>;

  game_level_object_generator(int cellSize, int columnCount, int rowCount);
  auto InsertInto(std::back_insert_iterator<asteroid_collection> inserter) const -> void;
  auto InsertInto(std::back_insert_iterator<target_collection> inserter) const -> void;

private:

  struct cell_id
  {
    int column;
    int row;

    auto operator<=>(const cell_id& cellId) const = default;
  };

  using cell_id_collection = std::set<cell_id>;

  [[nodiscard]] auto Create(int gridX, int gridY) const -> game_closed_object;

  auto GetCellsGreaterThan(float noiseValue, std::insert_iterator<cell_id_collection> inserter) const -> void;
  auto GetCellsLessThan(float noiseValue, std::insert_iterator<cell_id_collection> inserter) const -> void;
  auto GetCells(std::insert_iterator<cell_id_collection> inserter, std::function<bool(float)> noiseValueCheck) const -> void;

  int m_cellSize = 0;
  int m_columnCount = 0;
  int m_rowCount = 0;
};
