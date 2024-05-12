#pragma once

#include "framework.h"
#include "level_boundary.h"
#include "level_cell_collection.h"

class demo_level
{

public:

  using cell_collection = std::set<std::tuple<int, int>>;

  demo_level();

  constexpr static [[nodiscard]] auto CellWidth() noexcept -> int { return 400; }
  constexpr static [[nodiscard]] auto CellHeight() noexcept -> int { return 400; }

  [[nodiscard]] auto Cells() const noexcept -> const cell_collection&;
  [[nodiscard]] auto Portals() const noexcept -> const cell_collection&;
  [[nodiscard]] auto PlayerStartCell() const noexcept -> POINT_2I;

private:

  [[nodiscard]] auto FirstPortal() const -> POINT_2I;

private:

  inline static std::array m_levelData { 
    std::string { "             " },
    std::string { "             " },
    std::string { "  000   000  " },
    std::string { "  000   000  " },
    std::string { "             " },
    std::string { "00000   00000" },
    std::string { "00000   00000" },
    std::string { "             " },
    std::string { "  000        " },
    std::string { "  00   000   " },
    std::string { "  00   000   " },
    std::string { "  00         " },
    std::string { "P 00         " }
  };

  cell_collection m_cells;
  cell_collection m_portals;

};

inline [[nodiscard]] auto demo_level::Cells() const noexcept -> const cell_collection&
{
  return m_cells;
}

inline [[nodiscard]] auto demo_level::Portals() const noexcept -> const cell_collection&
{
  return m_portals;
}

inline auto demo_level::PlayerStartCell() const noexcept -> POINT_2I
{
  return m_portals.size() == 0 ? POINT_2I { 0, 0  } : FirstPortal();
}

inline auto demo_level::FirstPortal() const -> POINT_2I
{
  const auto& [column, row] = *std::begin(m_portals);
  return { column, row };
}
