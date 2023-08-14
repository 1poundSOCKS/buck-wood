#include "pch.h"
#include "screen_transform.h"

screen_transform::screen_transform()
{
  Set(D2D1::Matrix3x2F::Identity());
}

screen_transform::screen_transform(D2D1::Matrix3x2F transform)
{
  Set(transform);
}

auto screen_transform::Set(const D2D1::Matrix3x2F& transform) -> bool
{
  m_invertedTransform = m_transform = transform;
  return m_invertedTransform.Invert();
}

[[nodiscard]] auto screen_transform::Get() const -> const D2D1::Matrix3x2F&
{
  return m_transform;
}

[[nodiscard]] auto screen_transform::GetViewRect(D2D1_SIZE_F renderTargetSize) const -> D2D1_RECT_F
{
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };

  auto renderTargetLeft = renderTargetTopLeft.x;
  auto renderTargetTop = renderTargetTopLeft.y;
  auto renderTargetRight = renderTargetTopLeft.x;
  auto renderTargetBottom = renderTargetTopLeft.y;

  auto topLeft = m_invertedTransform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = m_invertedTransform.TransformPoint(renderTargetTopLeft);
  auto bottomLeft = m_invertedTransform.TransformPoint({renderTargetLeft, renderTargetBottom});
  auto topRight = m_invertedTransform.TransformPoint({renderTargetRight, renderTargetTop});

  auto [minX, minY] = std::minmax({topLeft.x, bottomRight.x, bottomLeft.x, topRight.x});
  auto [maxX, maxY] = std::minmax({topLeft.y, bottomRight.y, bottomLeft.y, topRight.y});

  return { minX, minY, maxX, maxY };
}
