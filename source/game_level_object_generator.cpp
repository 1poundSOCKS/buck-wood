#include "pch.h"
#include "game_level_object_generator.h"
#include "framework.h"
#include "perlin_simplex_noise.h"

game_level_object_generator::game_level_object_generator(int cellSize, int columnCount, int rowCount) : 
  m_cellSize(cellSize), m_columnCount(columnCount), m_rowCount(rowCount)
{
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<asteroid_collection> inserter) const -> void
{
  cell_id_collection cells;
  GetCellsLessThan(-0.9f, std::inserter(cells, cells.end()));

  for( auto cell : cells )
  {
    inserter = CreateAsteroid(m_columnCount / 2 - cell.column, m_rowCount / 2 - cell.row);
  }
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<target_collection> inserter) const -> void
{
  cell_id_collection cells;
  GetCellsGreaterThan(0.965f, std::inserter(cells, cells.end()));

  for( auto cell : cells )
  {
    auto x = static_cast<float>(m_columnCount / 2 - cell.column) * static_cast<float>(m_cellSize);
    auto y = static_cast<float>(m_rowCount / 2 - cell.row) * static_cast<float>(m_cellSize);

    inserter = level_target { x, y };
  }
}

auto game_level_object_generator::CreateAsteroid(int gridX, int gridY) const -> game_closed_object
{
  auto radius = static_cast<float>(m_cellSize / 2);
  auto x = static_cast<float>(gridX) * static_cast<float>(m_cellSize);
  auto y = static_cast<float>(gridY) * static_cast<float>(m_cellSize);

  std::vector<game_point> points;

  for( int angle = 0; angle < 360; angle += 40 )
  {
    auto angleInRadians = DEGTORAD(angle);

    auto cx = radius * sin(angleInRadians);
    auto cy = radius * cos(angleInRadians);

    auto noise = psn::GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy));
    noise = ( noise + 5.0f ) / 6.0f;

    points.emplace_back( game_point {x + cx * noise, y + cy * noise} );
  }

  game_closed_object asteroid;
  asteroid.Load(points.cbegin(), points.cend());

  return asteroid;
}

auto game_level_object_generator::GetCellsGreaterThan(float noiseValue, std::insert_iterator<cell_id_collection> inserter) const -> void
{
  GetCells(inserter, [noiseValue](float noise) -> bool { return noise > noiseValue; });
}

auto game_level_object_generator::GetCellsLessThan(float noiseValue, std::insert_iterator<cell_id_collection> inserter) const -> void
{
  GetCells(inserter, [noiseValue](float noise) -> bool { return noise < noiseValue; });
}

auto game_level_object_generator::GetCells(std::insert_iterator<cell_id_collection> inserter, std::function<bool(float)> noiseValueCheck) const -> void
{
  for( int column = 0; column < m_columnCount; ++column )
  {
    for( int row = 0; row < m_rowCount; ++row )
    {
      if( column != 0 && row != 0)
      {
        auto noise = psn::GetNoise(static_cast<float>(column), static_cast<float>(row));

        if( noiseValueCheck(noise) )
        {
          inserter = { column, row };
        }
      }
    }
  }
}
