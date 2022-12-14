#ifndef _sound_buffer_player_
#define _sound_buffer_player_

#include "sound_buffer.h"

struct sound_buffer_player
{
  sound_buffer_player(IDirectSoundBuffer8* soundBuffer);
  ~sound_buffer_player();

  void Play() const;
  void PlayOnLoop() const;
  void Stop() const;

  IDirectSoundBuffer8* soundBuffer;
};

void PlaySoundBuffer(IDirectSoundBuffer8* soundBuffer, bool loop=false);
void StopSoundBufferPlay(IDirectSoundBuffer8* soundBuffer);
void ResetSoundBuffer(IDirectSoundBuffer8* soundBuffer);

#endif
