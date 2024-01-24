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

  static auto StopMainMenuTheme() -> void
  {
    HRESULT hr = m_instance->m_menuTheme->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopMainMenuTheme");
  }

  static auto StartGameplayTheme() -> void {}
  static auto StopGameplayTheme() -> void {}

  static auto StartPlayerThruster() -> void {}
  static auto StopPlayerThruster() -> void {}
  static auto PlayerShot() -> void {}
  static auto TargetActivated() -> void {}
  static auto Explosion() -> void {}

private:

  audio_events() : 
    m_menuTheme { xaudio2_engine::get_raw(), audio_data::get(audio_data_item::menu_theme).format }
  {
    auto xaudio2Buffer = GetMusicBuffer(audio_data::get(audio_data_item::menu_theme));
    HRESULT hr = m_menuTheme.SubmitSourceBuffer(&xaudio2Buffer, nullptr);
    com_logger::write(log::type::debug, hr, "[audio_events] SubmitSourceBuffer");
  }

  static [[nodiscard]] auto GetMusicBuffer(audio_data::buffer audioBuffer) -> XAUDIO2_BUFFER
  {
    XAUDIO2_BUFFER xaudio2Buffer;
    xaudio2Buffer.Flags = 0;
    xaudio2Buffer.AudioBytes = audioBuffer.size;
    xaudio2Buffer.pAudioData = audioBuffer.data.get();
    xaudio2Buffer.PlayBegin = 0;
    xaudio2Buffer.PlayLength = 0;
    xaudio2Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    xaudio2Buffer.LoopBegin = 0;
    xaudio2Buffer.LoopLength = 0;
    xaudio2Buffer.pContext = nullptr;
    return xaudio2Buffer;
  }

private:

  inline static audio_events* m_instance { nullptr };
  xaudio2_source_voice m_menuTheme;

};
