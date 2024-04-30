#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  // for( int row = -2; row <= 2; ++row )
  // {
  //   for( int column = -2; column <= 2; ++column )
  //   {
  //     m_cells.emplace(column, row);
  //   }
  // }

  // for( int row = -6; row <= -4; ++row )
  // {
  //   for( int column = -4; column <= 4; ++column )
  //   {
  //     m_cells.emplace(column, row);
  //   }
  // }

  // m_cells.emplace(0, -3);
  auto rowCount = m_levelData.size();
  auto columnCount = m_levelData[0].size();

  auto centreRow = rowCount / 2;
  auto centreColumn = columnCount / 2;

  for( auto rowIndex  = 0; rowIndex < m_levelData.size(); ++rowIndex )
  {
    const auto& rowData = m_levelData[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      switch( rowData[columnIndex] )
      {
        case 1:
          m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
          break;
      }
    }
  }
}
