#pragma once

#include "com_singleton.h"

class primary_sound_buffer : public com_singleton<IDirectSoundBuffer>
{

public:

  static auto create(IDirectSound8* directSound) -> void;

private:

  primary_sound_buffer(IDirectSound8* directSound);

};
