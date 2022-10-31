#include "music_player.h"

music_player::music_player(const sound_buffer& soundBuffer) : soundBuffer(soundBuffer)
{
}

music_player::~music_player()
{
  Stop();
}

void music_player::Start()
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer.buffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    soundBuffer.buffer->SetCurrentPosition(0);
    soundBuffer.buffer->Play(0, 0, DSBPLAY_LOOPING);
  }
}

void music_player::Stop()
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffer.buffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
    soundBuffer.buffer->Stop();
}
