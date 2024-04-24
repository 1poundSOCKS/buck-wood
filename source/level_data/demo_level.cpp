#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  for( int row = -1; row <= 1; ++row )
  {
    for( int column = -1; column <= 1; ++column )
    {
      m_cells.emplace(column, row);
    }
  }
}
