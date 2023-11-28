#pragma once

#include "com_singleton.h"

class audio_output : public com_singleton<IDirectSoundBuffer>
{

public:

  static auto create(IDirectSound8* directSound) -> void;

private:

  audio_output(IDirectSound8* directSound);

};
