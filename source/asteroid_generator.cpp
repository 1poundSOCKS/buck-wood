#include "pch.h"
#include "asteroid_generator.h"
#include "framework.h"

asteroid_generator::asteroid_generator(int cellSize) : m_cellSize(static_cast<float>(cellSize))
{  
}

auto asteroid_generator::Create(int gridX, int gridY) -> game_closed_object
{
  std::array<game_point, 4> shape =
  {
    game_point { -100.0f, -100.0f, },
    game_point { 100.0f, -100.0f, },
    game_point { 100.0f, 100.0f, },
    game_point { -100.0f, 100.0f, }
  };

  auto angleInDegrees = GenerateAngle();

  auto x = static_cast<float>(gridX) * 300.0f;
  auto y = static_cast<float>(gridY) * 300.0f;

  auto rotationXform = D2D1::Matrix3x2F::Rotation(angleInDegrees);
  auto translationXform = D2D1::Matrix3x2F::Translation(x, y);

  std::vector<game_point> transformedShape;
  TransformPoints(shape.cbegin(), shape.cend(), std::back_inserter(transformedShape), rotationXform * translationXform);

  game_closed_object asteroid;
  asteroid.Load(transformedShape.cbegin(), transformedShape.cend());
  return asteroid;
}

[[nodiscard]] auto asteroid_generator::GenerateAngle() -> float
{
  return static_cast<float>(m_asteroidAngleDist(framework::rng()));
}
