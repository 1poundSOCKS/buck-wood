#include "pch.h"
#include "main_menu_screen_state.h"
#include "render.h"
#include "screen_render.h"
#include "screen_render_data.h"

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const main_menu_screen_state& screenState)
{
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( screenState.viewState == main_menu_screen_state::view_exit )
  {
    RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"save changes (y/n)", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    return;
  }

  std::wstring titleText;
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], titleText, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  RenderMouseCursor(renderTarget, renderBrushSelector[white], screenState.renderTargetMouseData.x, screenState.renderTargetMouseData.y);
}
