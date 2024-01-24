#pragma once

#include "xaudio2_source_voice.h"
#include "audio_data.h"
#include "com_logger.h"

class audio_player
{

public:

  enum class type { music, effect };

  audio_player(audio_data_item audioDataItem, type audioType) : m_sourceVoice { xaudio2_engine::get_raw(), audio_data::get(audioDataItem).format }
  {
    auto xaudio2Buffer = FormatBuffer(audio_data::get(audioDataItem), audioType);
    HRESULT hr = m_sourceVoice.SubmitSourceBuffer(&xaudio2Buffer, nullptr);
    com_logger::write(log::type::debug, hr, "[audio_events] SubmitSourceBuffer");
  }

  [[nodiscard]] auto operator->() const -> IXAudio2SourceVoice*
  {
    return m_sourceVoice.get();
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

  xaudio2_source_voice m_sourceVoice;

};
