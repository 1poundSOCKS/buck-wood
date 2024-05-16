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

  [[nodiscard]] auto Cells() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto Portals() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto Enemies1() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto Enemies2() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto PlayerStartCell() const noexcept -> POINT_2I override;

  auto Visit(auto&& visitor) -> void;

private:

  [[nodiscard]] auto FirstPortal() const -> POINT_2I;

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

inline [[nodiscard]] auto level_2::Cells() const noexcept -> const cell_collection&
{
  return m_cells;
}

inline [[nodiscard]] auto level_2::Portals() const noexcept -> const cell_collection&
{
  return m_portals;
}

inline [[nodiscard]] auto level_2::Enemies1() const noexcept -> const cell_collection&
{
  return m_enemies1;
}

inline [[nodiscard]] auto level_2::Enemies2() const noexcept -> const cell_collection&
{
  return m_enemies2;
}

inline auto level_2::PlayerStartCell() const noexcept -> POINT_2I
{
  return m_portals.size() == 0 ? POINT_2I { 0, 0  } : FirstPortal();
}

inline auto level_2::FirstPortal() const -> POINT_2I
{
  const auto& [column, row] = *std::begin(m_portals);
  return { column, row };
}

auto level_2::Visit(auto&& visitor) -> void
{
  // auto rowCount = m_levelData.size();
  // auto columnCount = m_levelData[0].length();

  // auto centreRow = rowCount / 2;
  // auto centreColumn = columnCount / 2;

  for( auto rowIndex  = 0; rowIndex < m_levelData.size(); ++rowIndex )
  {
    const auto& rowData = m_levelData[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      visitor(columnIndex, rowIndex, rowData[columnIndex]);
      // switch( rowData[columnIndex] )
      // {
        // case '1':
        //   m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   m_enemies1.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   break;
        // case '2':
        //   m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   m_enemies2.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   break;
        // case 'P':
        //   m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   m_portals.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   break;
        // case ' ':
        //   m_cells.emplace(static_cast<int>(columnIndex - centreColumn), static_cast<int>(rowIndex - centreRow));
        //   break;
      // }
    }
  }
}
