#include "main_menu_screen.h"

game_state::game_state()
{
}

void RenderFrame(const d2d_frame& frame, game_state& gameState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  std::wstring titleText = L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";
  titleText += L"\nPress F1 for level editor";

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(titleText.c_str(),titleText.length(), frame.textFormats->menuTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
}

screen_type UpdateState(game_state& gameState, const control_state& controlState, const system_timer& timer)
{
  if( gameState.starting )
  {
    gameState.starting = false;
    return screen_menu;
  }

  if( controlState.quitPress ) return screen_none;

  if( controlState.startGame ) return screen_play;

  return screen_menu;
}

void UpdateSound(const sound_buffers& soundBuffers, const game_state& gameState)
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
