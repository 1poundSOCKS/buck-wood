#include "pch.h"
#include "render_target_view.h"

render_target_view::render_target_view()
{
  SetTransform(D2D1::Matrix3x2F::Identity());
}

auto render_target_view::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
}

auto render_target_view::SetTransform(const D2D1::Matrix3x2F& transform) -> bool
{
  m_invertedTransform = m_transform = transform;
  return m_invertedTransform.Invert();
}

[[nodiscard]] auto render_target_view::GetTransform() const -> const D2D1::Matrix3x2F&
{
  return m_transform;
}

[[nodiscard]] auto render_target_view::GetViewRect() const -> D2D1_RECT_F
{
  auto renderTargetSize = m_renderTarget->GetSize();
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };

  auto topLeft = m_invertedTransform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = m_invertedTransform.TransformPoint(renderTargetBottomRight);
  return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
}

[[nodiscard]] auto render_target_view::GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data
{
  auto mousePosition = m_invertedTransform.TransformPoint({ screenInputState.renderTargetMouseData.x, screenInputState.renderTargetMouseData.y });
  auto previousMousePosition = m_invertedTransform.TransformPoint({ screenInputState.previousRenderTargetMouseData.x, screenInputState.previousRenderTargetMouseData.y });

  object_input_data objectInputData;
  objectInputData.SetMouseData({mousePosition.x, mousePosition.y, screenInputState.windowData.mouse.leftButtonDown, screenInputState.windowData.mouse.rightButtonDown});
  objectInputData.SetPreviousMouseData({previousMousePosition.x, previousMousePosition.y, screenInputState.previousWindowData.mouse.leftButtonDown, screenInputState.previousWindowData.mouse.rightButtonDown});
  return objectInputData;
}
