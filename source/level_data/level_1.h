#pragma once

#include "framework.h"
#include "level_base.h"
#include "level_boundary.h"
#include "level_cell_collection.h"

class level_1 : public level_base
{

public:

  level_1();
  ~level_1() override;

  [[nodiscard]] auto Cells() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto Portals() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto Enemies1() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto Enemies2() const noexcept -> const cell_collection& override;
  [[nodiscard]] auto PlayerStartCell() const noexcept -> POINT_2I override;

private:

  [[nodiscard]] auto FirstPortal() const -> POINT_2I;

private:

  inline static std::array m_levelData { 
    std::string { "             " },
    std::string { "  1     2    " },
    std::string { "  000   000  " },
    std::string { "  000   000  " },
    std::string { "             " },
    std::string { "00000   00000" },
    std::string { "00000   00000" },
    std::string { "             " },
    std::string { "  000    2   " },
    std::string { "  00   000   " },
    std::string { "  00   000   " },
    std::string { "  00         " },
    std::string { "P 00     1   " }
  };

  cell_collection m_cells;
  cell_collection m_portals;
  cell_collection m_enemies1;
  cell_collection m_enemies2;

};

inline [[nodiscard]] auto level_1::Cells() const noexcept -> const cell_collection&
{
  return m_cells;
}

inline [[nodiscard]] auto level_1::Portals() const noexcept -> const cell_collection&
{
  return m_portals;
}

inline [[nodiscard]] auto level_1::Enemies1() const noexcept -> const cell_collection&
{
  return m_enemies1;
}

inline [[nodiscard]] auto level_1::Enemies2() const noexcept -> const cell_collection&
{
  return m_enemies2;
}

inline auto level_1::PlayerStartCell() const noexcept -> POINT_2I
{
  return m_portals.size() == 0 ? POINT_2I { 0, 0  } : FirstPortal();
}

inline auto level_1::FirstPortal() const -> POINT_2I
{
  const auto& [column, row] = *std::begin(m_portals);
  return { column, row };
}
