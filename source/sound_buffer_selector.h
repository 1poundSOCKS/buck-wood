#ifndef _sound_buffer_selector_
#define _sound_buffer_selector_

#include "framework.h"

enum sound_buffer_name { menu_theme=0, shoot, thrust, target_activated };

void LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, sound_buffers& soundBuffers);

using global_sound_buffer_selector = sound_buffer_selector<sound_buffer_name>;

#endif
