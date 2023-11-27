#include "pch.h"
#include "keyboard_device.h"
#include "directx_functions.h"

auto keyboard_device::create(HINSTANCE instance, HWND window) -> void
{
}

keyboard_device::keyboard_device(HINSTANCE instance, HWND window) : com_singleton { CreateKeyboard(instance, window) }
{
}
