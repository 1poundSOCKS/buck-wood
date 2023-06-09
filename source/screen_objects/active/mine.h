#pragma once

#include "transformed_path_geometry.h"
#include "active_body.h"

class mine
{

public:

  mine() = default;
  mine(float x, float y);

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Destroy() -> void;
  auto Update(int64_t tickCount, float x, float y) -> void;
  auto Update(int64_t tickCount) -> void;

private:

  auto UpdateGeometry() -> void;

  active_body m_body { 200 };
  bool m_destroyed { false };

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};
