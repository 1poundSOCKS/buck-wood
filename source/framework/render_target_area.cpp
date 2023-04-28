#include "pch.h"
#include "render_target_area.h"

render_target_area::render_target_area(ID2D1RenderTarget* renderTarget, float width, float height)
{
  m_rect = GetRenderTargetRect(renderTarget, width, height);
}

[[nodiscard]] auto render_target_area::GetRect() const -> D2D1_RECT_F
{
  return m_rect;
}

[[nodiscard]] auto render_target_area::GetRenderTargetRect(ID2D1RenderTarget* renderTarget, float width, float height) -> D2D1_RECT_F
{
  auto renderTargetSize = renderTarget->GetSize();
  auto borderWidth = renderTargetSize.width * (1.0f - width) / 2.0f;
  auto borderHeight = renderTargetSize.height * (1.0f - height) / 2.0f;
  return { borderWidth, borderHeight, renderTargetSize.width - borderWidth - 1, renderTargetSize.height - borderHeight - 1 };
}
