#include "pch.h"
#include "sound_data.h"
#include "audio_output.h"

sound_data* sound_data::m_soundData = nullptr;

size_t sound_data::menu_theme { 0 };
size_t sound_data::shoot { 0 };
size_t sound_data::thrust { 0 };
size_t sound_data::target_activated { 0 };
size_t sound_data::mine_exploded { 0 };

constexpr [[nodiscard]] auto sound_data::GetWavFilenames()
{
  return std::array {
    sound_data_item { L"main_theme.wav", menu_theme }, 
    sound_data_item { L"shoot_effect.wav", shoot },
    sound_data_item { L"thrust_effect.wav", thrust },
    sound_data_item { L"target_activated.wav", target_activated },
    sound_data_item { L"mine_exploded.wav", mine_exploded }
  };
}

auto sound_data::create(const std::wstring& path) -> void
{
  const auto& directSound = audio_output::directSound();
  m_soundData = new sound_data(directSound.get(), path);
}

sound_data::sound_data(IDirectSound8* directSound, const std::wstring& path)
{
  m_buffers.emplace_back();
  LoadSoundBuffers(directSound, path);
}

[[nodiscard]] auto sound_data::get(size_t index) -> const sound_buffer&
{
  return m_soundData->m_buffers[index];
}

auto sound_data::LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path) -> void
{
  std::filesystem::path soundFilePath = path;
  soundFilePath /= L"sound";

  auto filenames = GetWavFilenames();

  std::transform(filenames.cbegin(), filenames.cend(), std::back_inserter(m_buffers), [this, directSound, soundFilePath](const auto soundDataItem)
  {
    auto [filename, index] = soundDataItem;
    index = m_buffers.size();
    std::filesystem::path fullFilename { soundFilePath };
    fullFilename /= filename;
    wav_file_data soundData { fullFilename.c_str() };
    return sound_buffer { directSound, soundData };
  });
}
