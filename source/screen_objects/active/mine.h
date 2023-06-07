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
  auto Update(int64_t tickCount, std::ranges::input_range auto&& objects) -> void;

private:

  auto UpdateGeometry() -> void;

  active_body m_body { 100 };
  bool m_destroyed { false };

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};

auto mine::Update(int64_t tickCount, std::ranges::input_range auto&& objects) -> void
{
  auto position = m_body.Position();

  for( const auto& object : objects )
  {
    auto objectPosition = object.Position();
    auto angle = CalculateAngle(position.x, position.y, objectPosition.x, objectPosition.y);
    m_body.SetAngle(angle);
    m_body.Update(tickCount);
  }

  UpdateGeometry();
}
