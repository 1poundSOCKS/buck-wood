#include "pch.h"

#include "framework.h"
#include "main_menu_screen.h"
#include "renderers.h"
#include "command_line.h"
#include "game_clock.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib,"Dwrite")
#pragma comment(lib,"Dinput8.lib")
#pragma comment(lib,"Xinput.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"RuntimeObject.lib")
#pragma comment(lib,"Shell32.lib")

#if 0
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")
#endif

auto APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) -> int
{
  pseudo_random_generator::seed(static_cast<unsigned int>(performance_counter::QueryValue()));
  
  main_window::create(instance, cmdShow);
  render_target::create(main_window::handle());
  user_input::create(instance, main_window::handle());
  windows_message_loop::create(render_target::swapChain(), render_target::fps());
  dwrite_factory::create();
  diagnostics::create();
  audio_output::create(main_window::handle());
  renderer::create();

  game_clock::setMultiplier(2);

  command_line commandLine { cmdLine };

  if( commandLine.Contains(L"-u") )
  {
    render_target::unlockFrameRate();
  }

  if( !commandLine.Contains(L"-w") )
  {
    render_target::fullScreen();
  }

  windows_message_loop::openScreen<main_menu_screen>(user_input::keyboardReader(), render_target::isFrameRateUnlocked(), DIK_F12);

  renderer::destroy();
  audio_output::destroy();
  diagnostics::destroy();
  dwrite_factory::destroy();
  windows_message_loop::destroy();
  user_input::destroy();
  render_target::destroy();
  main_window::destroy();

  return 0;
}
