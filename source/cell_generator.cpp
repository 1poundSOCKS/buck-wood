#include "pch.h"
#include "cell_generator.h"
#include "perlin_simplex_noise.h"

cell_generator::cell_generator(game_rect rect, float cellWidth, float cellHeight) : m_rect(rect), m_cellWidth(cellWidth), m_cellHeight(cellHeight)
{
}

auto cell_generator::Get(std::back_insert_iterator<collection> inserter) const -> void
{
  auto cellIndexLeft = static_cast<int>(m_rect.topLeft.x / m_cellWidth);
  auto cellIndexRight = static_cast<int>(m_rect.bottomRight.x / m_cellWidth) - 1;

  auto cellIndexTop = static_cast<int>(m_rect.topLeft.y / m_cellHeight);
  auto cellIndexBottom = static_cast<int>(m_rect.bottomRight.y / m_cellHeight) - 1;

  for( int row = cellIndexTop; row <= cellIndexBottom; ++row )
  {
    for( int column = cellIndexLeft; column <= cellIndexRight; ++column )
    {
      inserter = { column, row };
    }
  }
}
