#include "pch.h"
#include "demo_level.h"
#include "direct2d_functions.h"

demo_level::demo_level()
{
  auto rowCount = m_levelData.size();
  auto columnCount = m_levelData[0].length();

  auto centreRow = rowCount / 2;
  auto centreColumn = columnCount / 2;

  for( auto rowIndex  = 0; rowIndex < m_levelData.size(); ++rowIndex )
  {
    const auto& rowData = m_levelData[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      switch( rowData[columnIndex] )
      {
        case ' ':
          m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
          break;
      }
    }
  }
}
