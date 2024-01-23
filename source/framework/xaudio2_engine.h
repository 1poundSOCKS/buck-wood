#pragma once

#include "com_singleton.h"
#include "xaudio2_functions.h"

class xaudio2_engine : public com_singleton<IXAudio2>
{

public:

  static auto create() -> void
  {
    m_instance = new xaudio2_engine();
  }

private:

  xaudio2_engine() : com_singleton(xaudio2::CreateEngine())
  {
  }

};
