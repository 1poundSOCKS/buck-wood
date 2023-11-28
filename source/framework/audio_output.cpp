#include "pch.h"
#include "audio_output.h"
#include "directx_functions.h"

auto audio_output::create(IDirectSound8* directSound) -> void
{
  destroy();
  m_instance = new audio_output { directSound };
}

audio_output::audio_output(IDirectSound8* directSound) : com_singleton { CreatePrimarySoundBuffer(directSound) }
{
}
