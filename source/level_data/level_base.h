#pragma once

#include "framework.h"

class level_base
{

public:

  using cell_collection = std::set<std::tuple<int, int>>;

  virtual ~level_base() = default;
  virtual [[nodiscard]] auto Cells() const noexcept -> const cell_collection& = 0;
  virtual [[nodiscard]] auto Portals() const noexcept -> const cell_collection& = 0;
  virtual [[nodiscard]] auto Enemies1() const noexcept -> const cell_collection& = 0;
  virtual [[nodiscard]] auto Enemies2() const noexcept -> const cell_collection& = 0;
  virtual [[nodiscard]] auto PlayerStartCell() const noexcept -> POINT_2I = 0;

};
