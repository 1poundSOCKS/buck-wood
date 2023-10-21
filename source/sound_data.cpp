#include "pch.h"
#include "sound_data.h"

sound_data* sound_data::m_soundData = nullptr;

auto sound_data::create(IDirectSound8* directSound, const std::wstring& path) -> void
{
  m_soundData = new sound_data(directSound, path);
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
    index = static_cast<int>(m_buffers.size());
    std::filesystem::path fullFilename { soundFilePath };
    fullFilename /= filename;
    wav_file_data soundData { fullFilename.c_str() };
    return sound_buffer { directSound, soundData };
  });
}
