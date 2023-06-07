#pragma once

#include "path_geometry.h"

class mine
{

public:

  mine() = default;
  mine(float x, float y);

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto Geometry() const -> const path_geometry&;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Destroy() -> void;

private:

  game_point m_position { 0, 0 };
  path_geometry m_geometry;
  bool m_destroyed { false };

};
