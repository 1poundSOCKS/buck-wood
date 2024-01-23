#pragma once

#include "framework.h"

enum class sound_data_item { menu_theme=0, main_theme };

class audio_data
{

public:

  struct _data
  {
    WAVEFORMATEX format;
    uint32_t size;
    std::shared_ptr<uint8_t[]> data;
  };

  static auto create(std::wstring folder) -> void
  {
    destroy();
    m_instance = new audio_data(folder);
  }

  static auto destroy() -> void
  {
    delete m_instance;
  }
  
  [[nodiscard]] static auto get(sound_data_item item) -> const _data&
  {
    return m_instance->m_items[item];
  }

private:

  [[nodiscard]] auto LoadWavFileData(std::wstring_view filename) -> _data
  {
    std::filesystem::path fullFilename = m_folder;
    fullFilename /= L"sound";
    fullFilename /= filename.data();
    wav_file_data wavFileData(fullFilename.c_str());
    return { GetWaveFormatFromWavFile(wavFileData.dataFormat), wavFileData.data->size, std::move(wavFileData.data->data) };
  }

  [[nodiscard]] auto LoadAllWavFileData()
  {
    return std::array {
      std::tuple<sound_data_item, _data>(sound_data_item::menu_theme, LoadWavFileData(L"menu_theme.wav")),
      std::tuple<sound_data_item, _data>(sound_data_item::main_theme, LoadWavFileData(L"main_theme.wav"))
    };
  }

  audio_data(std::wstring folder) : m_folder { folder }, m_items { LoadAllWavFileData() }
  {
  }

private:

  inline static audio_data* m_instance { nullptr };
  std::wstring m_folder;
  enum_collection<sound_data_item, _data> m_items;

};
