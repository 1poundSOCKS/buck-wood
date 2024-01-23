#pragma once

namespace xaudio2
{
  [[nodiscard]] auto CreateEngine() -> winrt::com_ptr<IXAudio2>;
  [[nodiscard]] auto CreateMasteringVoice(IXAudio2* xaudio2Engine) -> IXAudio2MasteringVoice*;
  [[nodiscard]] auto CreateSourceVoice(IXAudio2* engine, const WAVEFORMATEX& wavFormat) -> IXAudio2SourceVoice*;
}
