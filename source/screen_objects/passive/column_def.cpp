#include "pch.h"
#include "column_def.h"

column_def::column_def(const D2D1_RECT_F& area, size_t rows)
{
  m_rows.resize(rows);

  for( size_t rowIndex = 0; rowIndex < rows; ++rowIndex )
  {
    auto areaHeight = area.bottom - area.top;
    auto rowHeight = areaHeight / rows;
    
    auto rowTop = area.top + rowIndex * rowHeight;
    auto rowBottom = rowTop + rowHeight;

    m_rows[rowIndex] = { area.left, rowTop, area.right, rowBottom };
  }
}
