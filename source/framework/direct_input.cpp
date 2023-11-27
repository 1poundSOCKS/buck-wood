#include "pch.h"
#include "direct_input.h"
#include "directx_functions.h"

auto direct_input::create(HINSTANCE instance) -> void
{
  destroy();
  m_instance = new direct_input { instance };
}

direct_input::direct_input(HINSTANCE instance) : com_singleton { CreateDirectInput(instance) }
{
}
