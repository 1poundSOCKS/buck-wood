#ifndef _sound_buffer_player_
#define _sound_buffer_player_

#include "sound_buffer.h"

struct sound_buffer_player
{
  sound_buffer_player(const winrt::com_ptr<IDirectSoundBuffer8>& soundBuffer);
  ~sound_buffer_player();

  void Play() const;
  void PlayOnLoop() const;
  void Stop() const;

  winrt::com_ptr<IDirectSoundBuffer8> soundBuffer;
};

#endif
