#pragma once

#include "framework.h"
#include "level_base.h"
#include "level_boundary.h"
#include "level_cell_collection.h"

class level_2 : public level_base
{

public:

  level_2();
  ~level_2() override;

  auto Visit(auto&& visitor) -> void;

private:

  inline static std::array m_levelData { 
    std::string { "0000000000 2 " },
    std::string { "0000000000   " },
    std::string { "00000        " },
    std::string { "00000    0000" },
    std::string { "00000    0000" },
    std::string { "             " },
    std::string { "  00     2   " },
    std::string { "  00    00   " },
    std::string { "  00    00   " },
    std::string { "  00         " },
    std::string { "P 00     1   " }
  };

  cell_collection m_cells;
  cell_collection m_portals;
  cell_collection m_enemies1;
  cell_collection m_enemies2;

};

auto level_2::Visit(auto&& visitor) -> void
{
  for( auto rowIndex  = 0; rowIndex < m_levelData.size(); ++rowIndex )
  {
    const auto& rowData = m_levelData[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      visitor(columnIndex, rowIndex, rowData[columnIndex]);
    }
  }
}
