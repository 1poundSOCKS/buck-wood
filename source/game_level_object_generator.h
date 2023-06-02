#pragma once

#include "geometry.h"
#include "level_target.h"
#include "renderers.h"
#include "perlin_simplex_noise.h"

class game_level_object_generator
{

public:

  using star_collection = std::vector<game_point>;
  using asteroid_collection = std::vector<game_closed_object>;
  using target_collection = std::vector<level_target>;

  game_level_object_generator(int minColumn, int maxColumn, int columnWidth, int minRow, int maxRow, int rowHeight, float noiseLower, float noiseUpper, float noiseDial);
  auto InsertInto(std::back_insert_iterator<star_collection> starInserter) const -> void;
  auto InsertInto(auto inserter) const -> void;
  auto InsertInto(std::back_insert_iterator<target_collection> targetInserter) const -> void;

private:

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

auto game_level_object_generator::InsertInto(auto inserter) const -> void
{
  for( auto row = m_minRow; row <= m_maxRow; ++row )
  {
    for( auto column = m_minColumn; column <= m_maxColumn; ++ column )
    {
      auto x = static_cast<float>(column * m_columnWidth);
      auto y = static_cast<float>(row * m_rowHeight);

      auto noiseValue = psn::GetNoise(x / m_noiseDial, y / m_noiseDial);

      auto rowRadius = m_rowHeight / 2;
      auto columnRadius = m_columnWidth / 2;

      auto rect = game_rect { { x - columnRadius, y - rowRadius }, { x + columnRadius, y + rowRadius } };
      
      if( noiseValue >= m_noiseLower && noiseValue < m_noiseUpper )
      {
        inserter = level_asteroid { rect };
      }
    }
  }
}
