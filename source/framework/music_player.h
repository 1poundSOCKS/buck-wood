#ifndef _music_player_
#define _music_player_

#include "sound_buffer.h"

struct music_player
{
  music_player(const sound_buffer& soundBuffer);
  ~music_player();

  void Start();
  void Stop();

  const sound_buffer& soundBuffer;
};

#endif
