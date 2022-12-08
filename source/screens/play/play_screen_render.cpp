#include "pch.h"
#include "render.h"
#include "screen_render.h"
#include "play_screen_state.h"
#include "screen_render_data.h"
#include "level_render.h"

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes);

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  const bespoke_render_data& renderData,
  const play_screen_state& screenState)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  float renderScale = screenState.state == play_screen_state::STATE::state_playing ? 1.5 : 1.5;

  RenderLevel(renderTarget, renderData, *screenState.levelState, renderScale);

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  switch( screenState.state )
  {
    case play_screen_state::state_paused:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[srtf_play_screen_state], L"PAUSED", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_level_complete:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[srtf_play_screen_state], L"LEVEL COMPLETE", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_game_complete:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[srtf_play_screen_state], GetGameCompleteMsg(screenState.levelTimes), DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_player_dead:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[srtf_play_screen_state], L"DEAD", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
  }

  float levelTimeRemaining = GetTimeRemainingInSeconds(*screenState.levelTimer);
  static wchar_t timerText[64];
  swprintf(timerText, L"%.2f", levelTimeRemaining);
  RenderText(renderTarget, renderBrushSelector[yellow], textFormatSelector[srtf_play_screen_timer], timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
  
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
