#include "game_sound.h"

void UpdateSound(const sound_buffers& soundBuffers, const game_state& gameState, const game_events& events)
{
  DWORD bufferStatus = 0;

  if( gameState.screen == game_state::screen_title )
  {
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
  else if( gameState.screen == game_state::screen_play )
  {
    const play_state_ptr& playState = gameState.playState;

    if( SUCCEEDED(soundBuffers.menuTheme->buffer->GetStatus(&bufferStatus)) )
    {
      if( bufferStatus & DSBSTATUS_PLAYING ) soundBuffers.menuTheme->buffer->Stop();
    }

    if( SUCCEEDED(soundBuffers.thrust->buffer->GetStatus(&bufferStatus)) )
    {
      if( bufferStatus & DSBSTATUS_PLAYING )
      {
        if( !playState->player->thrusterOn ||
            playState->playerState == game_state::player_dead ||
            playState->levelState == game_state::level_complete )
        {
          soundBuffers.thrust->buffer->Stop();
        }
      }
      else
      {
        if( playState->player->thrusterOn )
        {
          soundBuffers.thrust->buffer->SetCurrentPosition(0);
          soundBuffers.thrust->buffer->Play(0, 0, DSBPLAY_LOOPING);
        }
      }
    }

    if( events.playerShot )
    {
      soundBuffers.shoot->buffer->SetCurrentPosition(0);
      soundBuffers.shoot->buffer->Play(0, 0, 0);
    }

    if( events.targetShot )
    {
      soundBuffers.targetActivated->buffer->SetCurrentPosition(0);
      soundBuffers.targetActivated->buffer->Play(0, 0, 0);
    }
  }
  else
  {
    if( SUCCEEDED(soundBuffers.menuTheme->buffer->GetStatus(&bufferStatus)) )
    {
      if( bufferStatus & DSBSTATUS_PLAYING ) soundBuffers.menuTheme->buffer->Stop();
    }

    if( SUCCEEDED(soundBuffers.thrust->buffer->GetStatus(&bufferStatus)) )
    {
      if( bufferStatus & DSBSTATUS_PLAYING ) soundBuffers.thrust->buffer->Stop();
    }
  }
}
