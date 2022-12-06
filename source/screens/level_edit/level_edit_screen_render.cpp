#include "pch.h"
#include "render.h"
#include "screen_render.h"
#include "screen_render_data.h"
#include "level_edit_screen_state.h"

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  const bespoke_render_data& renderData,
  const level_edit_screen_state& screenState)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( screenState.viewState == level_edit_screen_state::view_exit )
  {
    RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"save changes (y/n)", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    return;
  }

  renderTarget->SetTransform(screenState.viewTransform);

  std::vector<render_line> renderLines;
  CreateDragDropRenderLines(renderLines, *screenState.dragDropState, renderBrushSelector);
  RenderLines(renderTarget, renderLines.begin(), renderLines.end());

  std::vector<render_point> renderPoints;
  CreateDragDropRenderPoints(renderPoints, *screenState.dragDropState, renderBrushSelector);
  RenderPoints(renderTarget, renderPoints.begin(), renderPoints.end());

  renderTarget->SetTransform(D2D1::IdentityMatrix());
  RenderMouseCursor(renderTarget, renderBrushSelector[white], screenState.renderTargetMouseData.x, screenState.renderTargetMouseData.y);
}
