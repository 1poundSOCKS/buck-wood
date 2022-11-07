#include "main_menu_screen.h"
#include "render.h"

main_menu_screen_state::main_menu_screen_state(const global_state& globalState)
: globalState(globalState),
  brushes(globalState.brushes),
  textFormats(globalState.textFormats)
{
}

void RefreshControlState(main_menu_control_state& screenControlState, const control_state& controlState)
{
  screenControlState.quit = controlState.escapeKeyPress;
  screenControlState.startPlay = controlState.spacebarKeyPress;
  screenControlState.startLevelEditor = controlState.functionKey_1;

  screenControlState.cancelExit = controlState.escapeKeyPress;
  screenControlState.saveChanges = controlState.keyPress_y;
  screenControlState.discardChanges = controlState.keyPress_n;

  screenControlState.renderTargetMouseX = controlState.renderTargetMouseX;
  screenControlState.renderTargetMouseY = controlState.renderTargetMouseY;
}

void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( screenState.viewState == main_menu_screen_state::view_exit )
  {
    RenderMainScreenPrompt(frame.renderTarget, screenState.textFormats.menuTextFormat, screenState.brushes.brushLevelEndText, L"save changes (y/n)");
    return;
  }

  std::wstring titleText;

#ifdef USE_KEYBOARDFORSPIN
  titleText += L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
#endif
  
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(titleText.c_str(),titleText.length(), screenState.textFormats.menuTextFormat.get(), rect, screenState.brushes.brushLevelEndText.get());

  RenderMouseCursor(frame.renderTarget, screenState.mouseCursor, screenState.renderTargetMouseX, screenState.renderTargetMouseY, screenState.brushes);
}

void UpdateScreenState(main_menu_screen_state& screenState, const D2D1_SIZE_F& renderTargetSize, const main_menu_control_state& controlState, const system_timer& timer)
{
  if( screenState.viewState == main_menu_screen_state::view_exit )
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
    return;
  }

  if( controlState.quit )
  {
    screenState.viewState = main_menu_screen_state::view_exit;
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

void UpdateSound(const sound_buffers& soundBuffers, const main_menu_screen_state& gameState)
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffers.thrust->buffer->GetStatus(&bufferStatus)) )
  {
    if( bufferStatus & DSBSTATUS_PLAYING ) soundBuffers.thrust->buffer->Stop();
  }
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const main_menu_screen_state& screenState, const main_menu_control_state& controlState)
{
}
