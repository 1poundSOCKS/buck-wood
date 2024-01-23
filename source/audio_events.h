#pragma once

#include "xaudio2_source_voice.h"
#include "audio_data.h"
#include "com_logger.h"

class audio_events
{

public:

  static auto create() -> void
  {
    destroy();
    m_instance = new audio_events();
  }

  static auto destroy() -> void
  {
    delete m_instance;
    m_instance = nullptr;
  }

  static auto StartMainMenuTheme() -> void
  {
    HRESULT hr = m_instance->m_menuTheme->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] StartMainMenuTheme");
  }

  static auto StopMainMenuTheme() -> void {}

  static auto StartGameplayTheme() -> void {}
  static auto StopGameplayTheme() -> void {}

  static auto StartPlayerThruster() -> void {}
  static auto StopPlayerThruster() -> void {}
  static auto PlayerShot() -> void {}
  static auto TargetActivated() -> void {}
  static auto Explosion() -> void {}

private:

  audio_events() : m_menuTheme { xaudio2_engine::get_raw(), audio_data::get(sound_data_item::menu_theme).format }
  {
    const auto& data = audio_data::get(sound_data_item::menu_theme);
    XAUDIO2_BUFFER buffer;
    buffer.Flags = 0;
    buffer.AudioBytes = data.size;
    buffer.pAudioData = data.data.get();
    buffer.PlayBegin = 0;
    buffer.PlayLength = 0;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    buffer.LoopBegin = 0;
    buffer.LoopLength = 0;
    buffer.pContext = nullptr;
    HRESULT hr = m_menuTheme.SubmitSourceBuffer(&buffer, nullptr);
    com_logger::write(log::type::debug, hr, "[audio_events] SubmitSourceBuffer");
  }

private:

  inline static audio_events* m_instance { nullptr };
  xaudio2_source_voice m_menuTheme;

};
