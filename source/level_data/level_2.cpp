#include "pch.h"
#include "level_2.h"
#include "direct2d_functions.h"

level_2::level_2()
{
  // auto rowCount = m_levelData.size();
  // auto columnCount = m_levelData[0].length();

  // auto centreRow = rowCount / 2;
  // auto centreColumn = columnCount / 2;

  // for( auto rowIndex  = 0; rowIndex < m_levelData.size(); ++rowIndex )
  // {
  //   const auto& rowData = m_levelData[rowIndex];

  //   for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
  //   {
  //     switch( rowData[columnIndex] )
  //     {
  //       case '1':
  //         m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         m_enemies1.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         break;
  //       case '2':
  //         m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         m_enemies2.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         break;
  //       case 'P':
  //         m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         m_portals.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         break;
  //       case ' ':
  //         m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
  //         break;
  //     }
  //   }
  // }
}

level_2::~level_2()
{
}
