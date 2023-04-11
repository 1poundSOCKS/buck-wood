#include "pch.h"
#include "render.h"
#include "screen_render.h"
#include "play_screen_state.h"
#include "screen_render_data.h"
#include "level_render.h"

void RenderScreenStateText(ID2D1RenderTarget* renderTarget, const play_screen_state& screenState, const screen_render_data& renderData);
void RenderLevelTimer(ID2D1RenderTarget* renderTarget, const play_screen_state& screenState, const screen_render_data& renderData);
std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes);

void RenderFrame(ID2D1RenderTarget* renderTarget, const play_screen_state& screenState)
{
  auto renderData = screenState.renderData;

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

//  RenderLevel(renderTarget, renderData, *);
  screenState.levelState->RenderTo(renderTarget, renderData);

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  RenderScreenStateText(renderTarget, screenState, renderData);

  RenderLevelTimer(renderTarget, screenState, renderData);
  
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };
  RenderMouseCursor(renderTarget, renderBrushSelector[white], screenState.renderTargetMouseData.x, screenState.renderTargetMouseData.y);
}

void RenderScreenStateText(ID2D1RenderTarget* renderTarget, const play_screen_state& screenState, const screen_render_data& renderData)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  switch( screenState.mode )
  {
    case play_screen_state::paused:
      RenderText(
        renderTarget, 
        renderBrushSelector[cyan], 
        textFormatSelector[srtf_play_screen_state], 
        L"PAUSED", 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::level_complete:
      RenderText(
        renderTarget, 
        renderBrushSelector[cyan], 
        textFormatSelector[srtf_play_screen_state], 
        L"LEVEL COMPLETE", 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::game_complete:
      RenderText(
        renderTarget, 
        renderBrushSelector[cyan], 
        textFormatSelector[srtf_play_screen_state], 
        GetGameCompleteMsg(screenState.levelTimes), 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::player_dead:
      RenderText(
        renderTarget, 
        renderBrushSelector[cyan], 
        textFormatSelector[srtf_play_screen_state], 
        L"DEAD", 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
  }
}

void RenderLevelTimer(ID2D1RenderTarget* renderTarget, const play_screen_state& screenState, const screen_render_data& renderData)
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  float levelTimeRemaining = screenState.levelState->GetPlayTimeRemainingInSeconds();
  
  std::wstring timerText = std::format(L"{:.2f}", levelTimeRemaining);
  RenderText(
    renderTarget, 
    renderBrushSelector[yellow], 
    textFormatSelector[srtf_play_screen_timer], 
    timerText, 
    DWRITE_PARAGRAPH_ALIGNMENT_NEAR, 
    DWRITE_TEXT_ALIGNMENT_TRAILING);
}

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes)
{
  std::wstring msg;
  for( auto levelTime: levelTimes ) { msg += std::format(L"{:.2f}", levelTime); }
  return msg;
}
