#include "pch.h"
#include "level_state.h"
#include "game_objects.h"
#include "level_render_object.h"

[[nodiscard]] auto GetViewRect(ID2D1RenderTarget* renderTarget, const level_state& levelState) -> D2D1_RECT_F;

auto RenderLevel(ID2D1RenderTarget* renderTarget, const screen_render_data& renderData,const level_state& levelState) -> void
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->SetTransform(levelState.viewTransform);

  auto viewRect = GetViewRect(renderTarget, levelState);

  std::for_each(std::execution::seq, levelState.solidObjects.cbegin(), levelState.solidObjects.cend(), [renderTarget, viewRect](const auto& object)
  {
    object.RenderTo(renderTarget, viewRect);
  });

#ifdef __RENDER_GROUND_LINES
  RenderLines(renderTarget, levelState.staticRenderLines.cbegin(), levelState.staticRenderLines.cend());
#endif
}

auto GetViewRect(ID2D1RenderTarget* renderTarget, const level_state& levelState) -> D2D1_RECT_F
{
  auto renderTargetSize = renderTarget->GetSize();
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };
  auto topLeft = levelState.invertedViewTransform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = levelState.invertedViewTransform.TransformPoint(renderTargetBottomRight);
  return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
}
