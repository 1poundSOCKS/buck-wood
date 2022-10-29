#include "main_menu_screen.h"
#include "render.h"

main_menu_screen_state::main_menu_screen_state()
{
}

void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

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
  frame.renderTarget->DrawTextW(titleText.c_str(),titleText.length(), frame.textFormats.menuTextFormat.get(), rect, frame.brushes.brushLevelEndText.get());

  RenderMouseCursor(frame, screenState.mouseCursor);
}

void UpdateState(main_menu_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.startPlay = screenState.startLevelEdit = false;
  
  if( screenState.starting )
  {
    screenState.starting = false;
    return;
  }

  if( controlState.quitPress )
  {
    screenState.quit = true;
    return;
  }

  if( controlState.startGame )
  {
    screenState.startPlay = true;
    return;
  }

  if( controlState.functionKey_1 )
  {
    screenState.startLevelEdit = true;
    return;
  }
}

void UpdateSound(const sound_buffers& soundBuffers, const main_menu_screen_state& gameState)
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffers.menuTheme->buffer->GetStatus(&bufferStatus)) )
  {
    if( !(bufferStatus & DSBSTATUS_PLAYING) )
    {
      soundBuffers.menuTheme->buffer->SetCurrentPosition(0);
      soundBuffers.menuTheme->buffer->Play(0, 0, DSBPLAY_LOOPING);
    }
  }

  if( SUCCEEDED(soundBuffers.thrust->buffer->GetStatus(&bufferStatus)) )
  {
    if( bufferStatus & DSBSTATUS_PLAYING ) soundBuffers.thrust->buffer->Stop();
  }
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const main_menu_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnosticsData, controlState, perfData, timer);
}
