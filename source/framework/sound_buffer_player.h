#ifndef _sound_buffer_player_
#define _sound_buffer_player_

#include "sound_buffer.h"

class sound_buffer_player
{

public:

  sound_buffer_player(const sound_buffer& soundBuffer);
  ~sound_buffer_player();

  void Play() const;
  void PlayOnLoop() const;
  void Stop() const;

private:

  sound_buffer m_soundBuffer;

};

#endif
