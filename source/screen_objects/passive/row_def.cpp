#include "pch.h"
#include "row_def.h"

row_def::row_def(const D2D1_RECT_F& area, size_t columns)
{
  m_columns.resize(columns);

  for( size_t columnIndex = 0; columnIndex < columns; ++columnIndex )
  {
    auto areaWidth = area.right - area.left;
    auto columnWidth = areaWidth / columns;
    
    auto columnLeft = area.left + columnIndex * columnWidth;
    auto columnRight = columnLeft + columnWidth;

    m_columns[columnIndex] = { columnLeft, area.top, columnRight, area.bottom };
  }
}
