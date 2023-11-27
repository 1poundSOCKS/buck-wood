#include "pch.h"
#include "keyboard_device.h"
#include "directx_functions.h"

auto keyboard_device::create(IDirectInput8* directInput, HWND wnd) -> void
{
  destroy();
  m_instance = new keyboard_device { directInput, wnd };
}

keyboard_device::keyboard_device(IDirectInput8* directInput, HWND wnd) : com_singleton { CreateKeyboard(directInput, wnd) }
{
}
