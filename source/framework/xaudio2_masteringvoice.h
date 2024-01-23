#pragma once

#include "xaudio2_functions.h"

class xaudio2_masteringvoice
{

public:

  static auto create(IXAudio2* xaudio2Engine) -> void
  {
    destroy();
    m_instance = new xaudio2_masteringvoice(xaudio2Engine);
  }

  static auto destroy() -> void
  {
    delete m_instance;
    m_instance = nullptr;
  }

private:

  xaudio2_masteringvoice(IXAudio2* xaudio2Engine)
  {
    m_masteringVoice = xaudio2::CreateMasteringVoice(xaudio2Engine);
  }

private:

  inline static xaudio2_masteringvoice* m_instance { nullptr };
  IXAudio2MasteringVoice* m_masteringVoice { nullptr };

};
