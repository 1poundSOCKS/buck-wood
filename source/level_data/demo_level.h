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

private:

  inline static std::array m_levelData { 
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 },
    std::array { 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 },
    std::array { 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    std::array { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
  };

  cell_collection m_cells;

};

inline [[nodiscard]] auto demo_level::Cells() const noexcept -> const cell_collection&
{
  return m_cells;
}
