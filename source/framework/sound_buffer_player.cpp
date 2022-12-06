#include "pch.h"
#include "sound_buffer_player.h"

sound_buffer_player::sound_buffer_player(IDirectSoundBuffer8* soundBuffer) : soundBuffer(soundBuffer)
{
}

sound_buffer_player::~sound_buffer_player()
{
  Stop();
}

void sound_buffer_player::Play() const
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    soundBuffer->SetCurrentPosition(0);
    soundBuffer->Play(0, 0, 0);
  }
}

void sound_buffer_player::PlayOnLoop() const
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    soundBuffer->SetCurrentPosition(0);
    soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
  }
}

void sound_buffer_player::Stop() const
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
    StopSoundBufferPlay(soundBuffer);
}

void PlaySoundBuffer(IDirectSoundBuffer8* soundBuffer, bool loop)
{
  soundBuffer->SetCurrentPosition(0);
  soundBuffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
}

void StopSoundBufferPlay(IDirectSoundBuffer8* soundBuffer)
{
  soundBuffer->Stop();
}
