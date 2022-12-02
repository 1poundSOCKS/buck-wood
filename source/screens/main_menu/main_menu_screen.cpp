#include "pch.h"
#include "main_menu_screen.h"
#include "render.h"

void UpdateScreenExitState(main_menu_screen_state& screenState, const main_menu_control_state& controlState);

main_menu_screen_state::main_menu_screen_state(const system_timer& timer, const global_state& globalState)
: globalState(globalState),
  checkSaveOnExit(globalState.gameLevelDataIndexUpdated)
{
}

void RefreshControlState(main_menu_control_state& screenControlState, const control_state& controlState)
{
  screenControlState.quit = controlState.escapeKeyPress;
  screenControlState.startPlay = controlState.spacebarKeyPress;
  screenControlState.startLevelEditor = controlState.f1Press;

  screenControlState.cancelExit = controlState.escapeKeyPress;
  screenControlState.saveChanges = controlState.keyPress_y;
  screenControlState.discardChanges = controlState.keyPress_n;

  screenControlState.renderTargetMouseX = controlState.renderTargetMouseData.x;
  screenControlState.renderTargetMouseY = controlState.renderTargetMouseData.y;
}

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
}

void UpdateScreenState(main_menu_screen_state& screenState, const main_menu_control_state& controlState, const system_timer& timer)
{
  if( screenState.viewState == main_menu_screen_state::view_exit )
  {
    UpdateScreenExitState(screenState, controlState);
    return;
  }

  if( controlState.quit )
  {
    if( screenState.checkSaveOnExit ) screenState.viewState = main_menu_screen_state::view_exit;
    else screenState.quit = true;
    return;
  }

  screenState.renderTargetMouseX = controlState.renderTargetMouseX;
  screenState.renderTargetMouseY = controlState.renderTargetMouseY;
  
  screenState.startPlay = screenState.startLevelEdit = false;
  
  if( screenState.starting )
  {
    screenState.starting = false;
    return;
  }

  if( controlState.startPlay )
  {
    screenState.startPlay = true;
    return;
  }

  if( controlState.startLevelEditor )
  {
    screenState.startLevelEdit = true;
    return;
  }
}

void UpdateScreenExitState(main_menu_screen_state& screenState, const main_menu_control_state& controlState)
{
  if( controlState.cancelExit )
  {
    screenState.viewState = main_menu_screen_state::view_default;
  }
  else if( controlState.discardChanges )
  {
    screenState.saveGameLevelData = false;
    screenState.quit = true;
  }
  else if( controlState.saveChanges )
  {
    screenState.saveGameLevelData = true;
    screenState.quit = true;
  }
}

void PlaySoundEffects(const main_menu_screen_state& screenState, global_sound_buffer_selector_type soundBuffers)
{
  IDirectSoundBuffer8* musicBuffer = soundBuffers[menu_theme];

  DWORD bufferStatus = 0;

  if( SUCCEEDED(musicBuffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    musicBuffer->SetCurrentPosition(0);
    musicBuffer->Play(0, 0, 0);
  }
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const main_menu_screen_state& screenState, const main_menu_control_state& controlState)
{
}

bool ContinueRunning(const main_menu_screen_state& screenState)
{
  return screenState.quit || screenState.startPlay || screenState.startLevelEdit ? false : true;
}
