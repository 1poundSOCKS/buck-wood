#include "pch.h"
#include "primary_sound_buffer.h"
#include "directx_functions.h"

auto primary_sound_buffer::create(IDirectSound8* directSound) -> void
{
  destroy();
  m_instance = new primary_sound_buffer { directSound };
}

primary_sound_buffer::primary_sound_buffer(IDirectSound8* directSound) : com_singleton { CreatePrimarySoundBuffer(directSound) }
{
}
