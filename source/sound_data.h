#ifndef _screen_sound_data_
#define _screen_sound_data_

#include "framework.h"
#include "sound_buffer.h"

class sound_data
{
public:

  static size_t menu_theme;
  static size_t shoot;
  static size_t thrust;
  static size_t target_activated;
  static size_t mine_exploded;

  static auto create(IDirectSound8* directSound, const std::wstring& path) -> void;
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

inline constexpr [[nodiscard]] auto sound_data::GetWavFilenames()
{
  return std::array {
    sound_data_item { L"main_theme.wav", menu_theme }, 
    sound_data_item { L"shoot_effect.wav", shoot },
    sound_data_item { L"thrust_effect.wav", thrust },
    sound_data_item { L"target_activated.wav", target_activated },
    sound_data_item { L"mine_exploded.wav", mine_exploded }
  };
}

#endif
