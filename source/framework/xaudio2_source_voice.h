#pragma once

#include "xaudio2_functions.h"

class xaudio2_source_voice
{

public:

  xaudio2_source_voice(IXAudio2* engine, const WAVEFORMATEX& format) : m_voice { xaudio2::CreateSourceVoice(engine, format) }
  {
  }

  auto SubmitSourceBuffer(const XAUDIO2_BUFFER* buffer, const XAUDIO2_BUFFER_WMA* bufferWMA) -> HRESULT
  {
    return m_voice->SubmitSourceBuffer(buffer, bufferWMA);
  }

  [[nodiscard]] auto get() -> IXAudio2SourceVoice*
  {
    return m_voice;
  }

  [[nodiscard]]auto operator->() -> IXAudio2SourceVoice*
  {
    return m_voice;
  }

private:

  IXAudio2SourceVoice* m_voice { nullptr };

};
