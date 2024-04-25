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

  for( int row = -6; row <= -4; ++row )
  {
    for( int column = -4; column <= 4; ++column )
    {
      m_cells.emplace(column, row);
    }
  }

  m_cells.emplace(0, -3);
}
