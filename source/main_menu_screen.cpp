#include "pch.h"
#include "main_menu_screen.h"
#include "render.h"

#define ENABLE_MUSIC

void UpdateScreenExitState(main_menu_screen_state& screenState, const main_menu_control_state& controlState);

main_menu_screen_state::main_menu_screen_state(const d2d_app& app, const global_state& globalState)
: globalState(globalState),
  renderBrushes(globalState.renderBrushes),
  textFormats(globalState.renderTextFormats),
  checkSaveOnExit(globalState.gameLevelDataIndexUpdated),
  musicPlayer(*globalState.soundBuffers.menuTheme)
{
#ifdef ENABLE_MUSIC
  musicPlayer.PlayOnLoop();
#endif
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

void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( screenState.viewState == main_menu_screen_state::view_exit )
  {
    // RenderMainScreenPrompt(frame.renderTarget, screenState.textFormats.menuTextFormat, screenState.renderBrushes.brushCyan, L"save changes (y/n)");
    RenderMainScreenPrompt(screenState.renderBrushes, screenState.textFormats, L"save changes (y/n)");
    return;
  }

  std::wstring titleText;
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(titleText.c_str(),titleText.length(), screenState.textFormats.menuTextFormat.get(), rect, screenState.renderBrushes.brushCyan.get());
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

void PlaySoundEffects(const main_menu_screen_state& screenState)
{
  const auto& sounds = screenState.globalState.soundBuffers;

  DWORD bufferStatus = 0;

  if( SUCCEEDED(sounds.thrust->buffer->GetStatus(&bufferStatus)) )
  {
    if( bufferStatus & DSBSTATUS_PLAYING ) sounds.thrust->buffer->Stop();
  }
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const main_menu_screen_state& screenState, const main_menu_control_state& controlState)
{
}

screen_status GetScreenStatus(const main_menu_screen_state& screenState)
{
  if( screenState.quit || screenState.startPlay || screenState.startLevelEdit ) return screen_closed;
  else return screen_active;
}

void UpdateGlobalState(global_state& globalState, const main_menu_screen_state& screenState)
{
  if( screenState.saveGameLevelData ) SaveAllGameLevelData(*globalState.gameLevelDataIndex);
  
  if( screenState.startPlay ) globalState.currentScreenId = screen_play;
  else if( screenState.startLevelEdit ) globalState.currentScreenId = screen_level_editor;
  else if( screenState.quit ) globalState.currentScreenId = screen_none;
}
