#include "pch.h"
#include "sound_buffer_player.h"

sound_buffer_player::sound_buffer_player(const sound_buffer& soundBuffer) : soundBuffer(soundBuffer)
{
}

sound_buffer_player::~sound_buffer_player()
{
  Stop();
}

void sound_buffer_player::Play() const
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer.buffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    soundBuffer.buffer->SetCurrentPosition(0);
    soundBuffer.buffer->Play(0, 0, 0);
  }
}

void sound_buffer_player::PlayOnLoop() const
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer.buffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    soundBuffer.buffer->SetCurrentPosition(0);
    soundBuffer.buffer->Play(0, 0, DSBPLAY_LOOPING);
  }
}

void sound_buffer_player::Stop() const
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer.buffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
    soundBuffer.buffer->Stop();
}
