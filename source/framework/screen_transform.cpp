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

  auto topLeft = m_invertedTransform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = m_invertedTransform.TransformPoint(renderTargetBottomRight);
  return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
}

[[nodiscard]] auto screen_transform::GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data
{
  auto mousePosition = m_invertedTransform.TransformPoint({ screenInputState.renderTargetMouseData.x, screenInputState.renderTargetMouseData.y });
  auto previousMousePosition = m_invertedTransform.TransformPoint({ screenInputState.previousRenderTargetMouseData.x, screenInputState.previousRenderTargetMouseData.y });

  object_input_data objectInputData;
  objectInputData.SetMouseData({mousePosition.x, mousePosition.y, screenInputState.windowData.mouse.leftButtonDown, screenInputState.windowData.mouse.rightButtonDown});
  objectInputData.SetPreviousMouseData({previousMousePosition.x, previousMousePosition.y, screenInputState.previousWindowData.mouse.leftButtonDown, screenInputState.previousWindowData.mouse.rightButtonDown});
  return objectInputData;
}
