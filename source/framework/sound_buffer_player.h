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

void PlaySoundEffect(IDirectSoundBuffer8* soundBuffer, bool loop=false);

#endif
