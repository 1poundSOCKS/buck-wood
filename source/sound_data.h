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
};

inline constexpr [[nodiscard]] auto sound_data::GetWavFilenames()
{
  return std::array {
    L"main_theme.wav", 
    L"shoot_effect.wav", 
    L"thrust_effect.wav", 
    L"target_activated.wav",
    L"mine_exploded.wav"
  };
}

inline int sound_data::menu_theme { 0 };
inline int sound_data::shoot { 1 };
inline int sound_data::thrust { 2 };
inline int sound_data::target_activated { 3 };
inline int sound_data::mine_exploded { 4 };

#endif
