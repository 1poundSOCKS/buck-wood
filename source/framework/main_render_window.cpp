#include "pch.h"
#include "main_render_window.h"

main_render_window::main_render_window(HWND windowHandle, UINT fps) : renderTarget(windowHandle, fps)
{
}
