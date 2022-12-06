#ifndef _screen_sound_data_
#define _screen_sound_data_

#include "framework.h"

enum sound_buffer_name { menu_theme=0, shoot, thrust, target_activated };

void LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, sound_buffers& soundBuffers);

using global_sound_buffer_selector = sound_buffer_selector<sound_buffer_name>;

struct bespoke_sound_data
{
  sound_buffers soundBuffers;
};

#endif
