#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  for( int row = -2; row <= 2; ++row )
  {
    for( int column = -2; column <= 2; ++column )
    {
      m_cells.emplace(column, row);
    }
  }

  for( int row = -4; row <= 4; ++row )
  {
    m_cells.emplace(-4, row);
  }

  m_cells.emplace(-3, 0);
}
