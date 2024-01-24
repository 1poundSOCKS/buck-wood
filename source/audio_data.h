#pragma once

#include "framework.h"

enum class audio_data_item { menu_theme=0, main_theme, explosion, shoot, target_activated, thruster };

class audio_data
{

public:

  struct buffer
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
  
  [[nodiscard]] static auto get(audio_data_item item) -> const buffer&
  {
    return m_instance->m_items[item];
  }

private:

  [[nodiscard]] auto LoadWavFileData(std::wstring_view filename) -> buffer
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
      std::tuple<audio_data_item, buffer>(audio_data_item::menu_theme, LoadWavFileData(L"menu_theme.wav")),
      std::tuple<audio_data_item, buffer>(audio_data_item::main_theme, LoadWavFileData(L"main_theme.wav")),
      std::tuple<audio_data_item, buffer>(audio_data_item::explosion, LoadWavFileData(L"mine_exploded.wav")),
      std::tuple<audio_data_item, buffer>(audio_data_item::shoot, LoadWavFileData(L"shoot_effect.wav")),
      std::tuple<audio_data_item, buffer>(audio_data_item::target_activated, LoadWavFileData(L"target_activated.wav")),
      std::tuple<audio_data_item, buffer>(audio_data_item::thruster, LoadWavFileData(L"thrust_effect.wav"))
    };
  }

  audio_data(std::wstring folder) : m_folder { folder }, m_items { LoadAllWavFileData() }
  {
  }

private:

  inline static audio_data* m_instance { nullptr };
  std::wstring m_folder;
  enum_collection<audio_data_item, buffer> m_items;

};
