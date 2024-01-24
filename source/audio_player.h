#pragma once

#include "xaudio2_source_voice.h"
#include "audio_data.h"
#include "com_logger.h"

class audio_player
{

public:

  audio_player(audio_data_item audioDataItem) : m_sourceVoice { xaudio2_engine::get_raw(), audio_data::get(audioDataItem).format }
  {
    auto xaudio2Buffer = GetMusicBuffer(audio_data::get(audioDataItem));
    HRESULT hr = m_sourceVoice.SubmitSourceBuffer(&xaudio2Buffer, nullptr);
    com_logger::write(log::type::debug, hr, "[audio_events] SubmitSourceBuffer");
  }

  [[nodiscard]] auto operator->() const -> IXAudio2SourceVoice*
  {
    return m_sourceVoice.get();
  }

private:

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

  xaudio2_source_voice m_sourceVoice;

};
