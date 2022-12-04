#include "pch.h"
#include "render.h"
#include "screen_render.h"
#include "play_screen_state.h"
#include "screen_render_data.h"

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes);

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const play_screen_state& screenState)
{
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderLevel(renderTarget, renderBrushSelector, *screenState.levelState);

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  switch( screenState.state )
  {
    case play_screen_state::state_paused:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"PAUSED", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_level_complete:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"LEVEL COMPLETE", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_game_complete:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], GetGameCompleteMsg(screenState.levelTimes), DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_player_dead:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"DEAD", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
  }

  float levelTimeRemaining = GetTimeRemainingInSeconds(*screenState.levelTimer);
  static wchar_t timerText[64];
  swprintf(timerText, L"%.2f", levelTimeRemaining);
  RenderText(renderTarget, renderBrushSelector[yellow], textFormatSelector[diagnostics], timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
  
  RenderMouseCursor(renderTarget, renderBrushSelector[white], screenState.renderTargetMouseData.x, screenState.renderTargetMouseData.y);
}

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes)
{
  std::wstring msg = L"";

  for( auto levelTime: levelTimes )
  {
    static wchar_t text[64];
    swprintf(text, L"%.2f\n", levelTime);
    msg += text;
  }

  return msg;
}
