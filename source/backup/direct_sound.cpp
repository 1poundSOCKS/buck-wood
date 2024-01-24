#include "pch.h"
#include "direct_sound.h"
#include "directx_functions.h"

auto direct_sound::create(HWND wnd) -> void
{
  destroy();
  m_instance = new direct_sound { wnd };
}

direct_sound::direct_sound(HWND wnd) : com_singleton { CreateDirectSound(wnd) }
{
}
