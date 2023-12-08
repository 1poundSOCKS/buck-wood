#pragma once

#include "geometry.h"

class screen_transform
{

public:

  screen_transform(D2D1::Matrix3x2F transform);
  auto Set(const D2D1::Matrix3x2F& transform) -> bool;
  [[nodiscard]] auto Get() const -> const D2D1::Matrix3x2F&;
  [[nodiscard]] auto GetViewRect(D2D1_SIZE_F renderTargetSize) const -> D2D1_RECT_F;
  [[nodiscard]] auto GetScreenPosition(const game_point& windowPosition) const -> game_point;

private:

  D2D1::Matrix3x2F m_transform;
  D2D1::Matrix3x2F m_invertedTransform;
};

[[nodiscard]] inline auto screen_transform::GetScreenPosition(const game_point& windowPosition) const -> game_point
{
  auto position = m_invertedTransform.TransformPoint({ windowPosition.x, windowPosition.y });
  return { position.x, position.y };
}
