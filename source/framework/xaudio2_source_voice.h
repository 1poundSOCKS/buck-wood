#pragma once

class xaudio2_source_voice
{

public:

  xaudio2_source_voice(IXAudio2SourceVoice* voice, const XAUDIO2_BUFFER& buffer) : m_voice { voice }
  {
    m_voice->SubmitSourceBuffer(&buffer, nullptr);
  }

private:

  IXAudio2SourceVoice* m_voice { nullptr };

};
