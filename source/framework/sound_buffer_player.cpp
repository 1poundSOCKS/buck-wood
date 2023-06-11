#include "pch.h"
#include "sound_buffer_player.h"

sound_buffer_player::sound_buffer_player(IDirectSoundBuffer8* soundBuffer) : soundBuffer(soundBuffer)
{
}

sound_buffer_player::~sound_buffer_player()
{
  if( soundBuffer )
  {
    StopSoundBufferPlay(soundBuffer);
    ResetSoundBuffer(soundBuffer);
  }
}

void sound_buffer_player::Play() const
{
  if( soundBuffer )
  {
    ResetSoundBuffer(soundBuffer);
    PlaySoundBuffer(soundBuffer);
  }
}

void sound_buffer_player::PlayOnLoop() const
{
  if( soundBuffer )
  {
    ResetSoundBuffer(soundBuffer);
    PlaySoundBuffer(soundBuffer, true);
  }
}

void PlaySoundBuffer(IDirectSoundBuffer8* soundBuffer, bool loop)
{
  if( soundBuffer )
  {
    DWORD bufferStatus = S_OK;

    if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
      soundBuffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
  }
}

void StopSoundBufferPlay(IDirectSoundBuffer8* soundBuffer)
{
  if( soundBuffer )
  {
    DWORD bufferStatus = S_OK;

    if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
      soundBuffer->Stop();
  }
}

void ResetSoundBuffer(IDirectSoundBuffer8* soundBuffer)
{
  if( soundBuffer )
  {
    soundBuffer->SetCurrentPosition(0);
  }
}
