#ifndef _sound_buffers_
#define _sound_buffers_

#include "framework.h"

// struct sound_buffers
// {
//   sound_buffers(IDirectSound8* directSound, const std::wstring& path);

//   winrt::com_ptr<IDirectSoundBuffer8> menuTheme;
//   winrt::com_ptr<IDirectSoundBuffer8> shoot;
//   winrt::com_ptr<IDirectSoundBuffer8> thrust;
//   winrt::com_ptr<IDirectSoundBuffer8> targetActivated;
// };

enum sound_buffer_name { menu_theme=0, shoot, thrust, target_activated };

void LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, sound_buffers& soundBuffers);

using global_sound_buffer_selector_type = sound_buffer_selector<sound_buffer_name>;

#endif
