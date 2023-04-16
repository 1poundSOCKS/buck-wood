#include "pch.h"
#include "main_menu_screen_state.h"
#include "screen_input_state.h"
#include "timers.h"
#include "screen_render.h"

void UpdateScreenState(main_menu_screen_state& screenState, const screen_input_state& screenInputState)
{
  screenState.Update(screenInputState);
}

void RenderFrame(ID2D1RenderTarget* renderTarget, const main_menu_screen_state& screenState)
{
  screenState.RenderTo(renderTarget);
}

bool ContinueRunning(const main_menu_screen_state& screenState)
{
  return screenState.ContinueRunning();
}

void FormatDiagnostics(const main_menu_screen_state& screenState, diagnostics_data_inserter_type diagnosticsDataInserter)
{
  screenState.FormatDiagnostics(diagnosticsDataInserter);
}

main_menu_screen_state::main_menu_screen_state(screen_render_data renderData)
: renderData(renderData)
{
}

auto main_menu_screen_state::Update(const screen_input_state& inputState) -> void
{
  mousePointerX = static_cast<float>(inputState.windowData.mouse.x);
  mousePointerY = static_cast<float>(inputState.windowData.mouse.y);
  
  if( viewState == view_exit )
  {
    UpdateScreenExitState(inputState);
    return;
  }

  if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    if( checkSaveOnExit )
      viewState = view_exit;
    else
      quit = true;
    
    return;
  }

  renderTargetMouseData = inputState.renderTargetMouseData;
  
  startPlay = startLevelEdit = false;
  
  if( starting )
  {
    starting = false;
    return;
  }

  if( KeyPressed(inputState, DIK_SPACE) )
  {
    startPlay = true;
    return;
  }

  if( KeyPressed(inputState, DIK_F1) )
  {
    startLevelEdit = true;
    return;
  }
}

auto main_menu_screen_state::RenderTo(ID2D1RenderTarget* renderTarget) const -> void
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( viewState == main_menu_screen_state::view_exit )
  {
    RenderText(
      renderTarget, 
      renderBrushSelector[cyan], 
      textFormatSelector[srtf_prompt], 
      L"save changes (y/n)", 
      DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
      DWRITE_TEXT_ALIGNMENT_CENTER);

    return;
  }

  std::wstring titleText;
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  RenderText(
    renderTarget, 
    renderBrushSelector[cyan], 
    textFormatSelector[srtf_main_menu], 
    titleText, 
    DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
    DWRITE_TEXT_ALIGNMENT_CENTER);
    
  RenderMouseCursor(renderTarget, renderBrushSelector[white], renderTargetMouseData.x, renderTargetMouseData.y);
}

auto main_menu_screen_state::PlaySoundEffects() const -> void
{
}

[[nodiscard]] auto main_menu_screen_state::ContinueRunning() const -> bool
{
  return quit || startPlay || startLevelEdit ? false : true;
}

auto main_menu_screen_state::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
}

auto main_menu_screen_state::StartPlay() const -> bool
{
  return startPlay;
}

auto main_menu_screen_state::SaveGameLevelData() const -> bool
{
  return saveGameLevelData;
}

auto main_menu_screen_state::UpdateScreenExitState(const screen_input_state& screenInputState) -> void
{
  if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    viewState = view_default;
  }
  else if( KeyPressed(screenInputState, DIK_N) )
  {
    saveGameLevelData = false;
    quit = true;
  }
  else if( KeyPressed(screenInputState, DIK_Y) )
  {
    saveGameLevelData = true;
    quit = true;
  }
}
