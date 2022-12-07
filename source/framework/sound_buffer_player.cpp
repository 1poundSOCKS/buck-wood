#include "pch.h"
#include "sound_buffer_player.h"

sound_buffer_player::sound_buffer_player(IDirectSoundBuffer8* soundBuffer) : soundBuffer(soundBuffer)
{
}

sound_buffer_player::~sound_buffer_player()
{
  StopSoundBufferPlay(soundBuffer);
  ResetSoundBuffer(soundBuffer);
}

void sound_buffer_player::Play() const
{
  ResetSoundBuffer(soundBuffer);
  PlaySoundBuffer(soundBuffer);
}

void sound_buffer_player::PlayOnLoop() const
{
  ResetSoundBuffer(soundBuffer);
  PlaySoundBuffer(soundBuffer, true);
}

void PlaySoundBuffer(IDirectSoundBuffer8* soundBuffer, bool loop)
{
  DWORD bufferStatus = S_OK;

  if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
    soundBuffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
}

void StopSoundBufferPlay(IDirectSoundBuffer8* soundBuffer)
{
  DWORD bufferStatus = S_OK;

  if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
    soundBuffer->Stop();
}

void ResetSoundBuffer(IDirectSoundBuffer8* soundBuffer)
{
  soundBuffer->SetCurrentPosition(0);
}
