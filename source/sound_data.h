#ifndef _screen_sound_data_
#define _screen_sound_data_

#include "framework.h"
#include "sound_buffer.h"

class sound_data
{
public:

  static int menu_theme;
  static int shoot;
  static int thrust;
  static int target_activated;
  static int mine_exploded;

  static auto create(IDirectSound8* directSound, const std::wstring& path) -> void;
  [[nodiscard]] static auto get(size_t index) -> const sound_buffer&;

private:

  static constexpr [[nodiscard]] auto GetWavFilenames();
  
  auto LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path) -> void;

  static sound_data* m_soundData;

  sound_data(IDirectSound8* directSound, const std::wstring& path);

  using sound_buffer_collection = std::vector<sound_buffer>;
  sound_buffer_collection m_buffers;

  using sound_data_item = std::tuple<LPCWSTR, int&>;
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

inline int sound_data::menu_theme { -1 };
inline int sound_data::shoot { -1 };
inline int sound_data::thrust { -1 };
inline int sound_data::target_activated { -1 };
inline int sound_data::mine_exploded { -1 };

#endif
