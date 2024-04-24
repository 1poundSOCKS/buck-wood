#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  for( int row = -4; row <= 4; ++row )
  {
    for( int column = -4; column <= 4; ++column )
    {
      m_cells.emplace(column, row);
    }
  }
}
