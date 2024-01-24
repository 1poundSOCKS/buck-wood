#pragma once

#include "xaudio2_source_voice.h"
#include "audio_data.h"
#include "com_logger.h"

class audio_player
{

public:

  enum class type { music, effect };

  audio_player(audio_data_item audioDataItem, type audioType) : 
    m_type { audioType }, m_sourceVoice { xaudio2_engine::get_raw(), audio_data::get(audioDataItem).format }, m_buffer { FormatBuffer(audio_data::get(audioDataItem), audioType) }
  {
  }

  [[nodiscard]] auto operator->() const -> IXAudio2SourceVoice*
  {
    return m_sourceVoice.get();
  }

  auto Start() -> void
  {
    HRESULT hr;

    switch( m_type )
    {
      case type::music:
        hr = m_sourceVoice.SubmitSourceBuffer(&m_buffer, nullptr);
        hr = m_sourceVoice->Start(0);
        com_logger::write(log::type::debug, hr, "[audio_events] SubmitSourceBuffer");
        break;
      case type::effect:
        hr = m_sourceVoice->Stop();
        hr = m_sourceVoice->FlushSourceBuffers();
        hr = m_sourceVoice.SubmitSourceBuffer(&m_buffer, nullptr);
        hr = m_sourceVoice->Start(0);
        com_logger::write(log::type::debug, hr, "[audio_events] SubmitSourceBuffer");
        break;
    }
  }

private:

  static [[nodiscard]] auto FormatBuffer(audio_data::buffer audioBuffer, type audioType) -> XAUDIO2_BUFFER
  {
    XAUDIO2_BUFFER xaudio2Buffer;
    xaudio2Buffer.Flags = 0;
    xaudio2Buffer.AudioBytes = audioBuffer.size;
    xaudio2Buffer.pAudioData = audioBuffer.data.get();
    xaudio2Buffer.PlayBegin = 0;
    xaudio2Buffer.PlayLength = 0;
    xaudio2Buffer.LoopCount = audioType == type::music ? XAUDIO2_LOOP_INFINITE : 0;
    xaudio2Buffer.LoopBegin = 0;
    xaudio2Buffer.LoopLength = 0;
    xaudio2Buffer.pContext = nullptr;
    return xaudio2Buffer;
  }

private:

  type m_type;
  xaudio2_source_voice m_sourceVoice;
  XAUDIO2_BUFFER m_buffer;

};
