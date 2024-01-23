#include "pch.h"
#include "xaudio2_functions.h"
#include "com_logger.h"

[[nodiscard]] auto xaudio2::CreateEngine() -> winrt::com_ptr<IXAudio2>
{
  winrt::com_ptr<IXAudio2> engine;
  HRESULT hr = XAudio2Create(engine.put(), 0, XAUDIO2_DEFAULT_PROCESSOR);
  com_logger::fatal(log::type::info, hr, "[xaudio2] XAudio2Create");
  return engine;
}

[[nodiscard]] auto xaudio2::CreateMasteringVoice(IXAudio2* xaudio2Engine) -> IXAudio2MasteringVoice*
{
  IXAudio2MasteringVoice* masteringVoice;
  HRESULT hr = xaudio2Engine->CreateMasteringVoice(&masteringVoice);
  com_logger::fatal(log::type::info, hr, "[xaudio2] CreateMasteringVoice");
  return masteringVoice;
}

[[nodiscard]] auto xaudio2::CreateSourceVoice(IXAudio2* engine, const WAVEFORMATEX& wavFormat) -> IXAudio2SourceVoice*
{
  IXAudio2SourceVoice* sourceVoice;
  HRESULT hr = engine->CreateSourceVoice(&sourceVoice, &wavFormat);
  com_logger::fatal(log::type::info, hr, "[xaudio2] CreateSourceVoice");
  return sourceVoice;
}
