#ifndef _screen_sound_data_
#define _screen_sound_data_

#include "framework.h"

enum sound_buffer_name { menu_theme=0, shoot, thrust, target_activated };

auto LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, std::back_insert_iterator<sound_buffers> soundBufferInserter) -> void;

using global_sound_buffer_selector = sound_buffer_selector<sound_buffer_name>;

class sound_data
{
public:

  static auto create(IDirectSound8* directSound, const std::wstring& path) -> void;
  static auto soundBuffers() -> sound_buffers&;

private:

  static sound_data* m_soundData;

  sound_data(IDirectSound8* directSound, const std::wstring& path);
  sound_buffers m_soundBuffers;
};

#endif
