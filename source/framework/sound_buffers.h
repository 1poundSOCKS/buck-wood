#pragma once

#include "sound_buffer.h"

class sound_buffers
{

public:

  sound_buffers() = default;
  template <size_t N> auto Load(IDirectSound8* directSound, const std::wstring& path, std::array<const wchar_t*, N> filenames) -> void;
  [[nodiscard]] auto operator[](size_t index) const -> const sound_buffer&;

private:

  using sound_buffer_collection = std::vector<sound_buffer>;
  sound_buffer_collection m_buffers;

};

[[nodiscard]] inline auto sound_buffers::operator[](size_t index) const -> const sound_buffer&
{
  return m_buffers[index];
}

template <size_t N> inline auto sound_buffers::Load(IDirectSound8* directSound, const std::wstring& path, std::array<const wchar_t*, N> filenames) -> void
{
  std::filesystem::path soundFilePath = path;
  soundFilePath /= L"sound";

  std::transform(filenames.cbegin(), filenames.cend(), std::back_inserter(m_buffers), [directSound, soundFilePath](const auto filename)
  {
    return sound_buffer { LoadSoundBuffer(directSound, soundFilePath, filename) };
  });
}
