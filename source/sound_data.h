#ifndef _screen_sound_data_
#define _screen_sound_data_

#include "sound_buffer.h"

class sound_data
{
public:

  static size_t menu_theme;
  static size_t shoot;
  static size_t thrust;
  static size_t target_activated;
  static size_t mine_exploded;

  static auto create(const std::wstring& path) -> void;
  static auto destroy() -> void;
  
  [[nodiscard]] static auto get(size_t index) -> const sound_buffer&;

private:

  static constexpr [[nodiscard]] auto GetWavFilenames();
  
  auto LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path) -> void;

  static sound_data* m_soundData;

  sound_data(IDirectSound8* directSound, const std::wstring& path);

  using sound_buffer_collection = std::vector<sound_buffer>;
  sound_buffer_collection m_buffers;

  using sound_data_item = std::tuple<LPCWSTR, size_t&>;
};

#endif
