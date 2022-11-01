#ifndef _sound_buffer_player_
#define _sound_buffer_player_

#include "sound_buffer.h"

struct sound_buffer_player
{
  sound_buffer_player(const sound_buffer& soundBuffer);
  ~sound_buffer_player();

  void Play() const;
  void PlayOnLoop() const;
  void Stop() const;

  const sound_buffer& soundBuffer;
};

#endif
