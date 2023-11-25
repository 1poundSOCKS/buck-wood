#include "pch.h"

#include "framework.h"
#include "main_menu_screen.h"
#include "renderers.h"
#include "command_line.h"
#include "game_clock.h"
#include "render_screen.h"

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
  swap_chain::create(main_window::handle(), 60, 1);
  swap_chain::get()->SetFullscreenState(FALSE, NULL);
  d2d_factory::create(); 
  render_target::create(swap_chain::get_raw(), d2d_factory::get_raw());
  user_input::create(instance, main_window::handle());
  windows_message_loop::create(swap_chain::get(), 60, DIK_F12);
  dwrite_factory::create();
  diagnostics::create();
  audio_output::create(main_window::handle());
  renderer::create();

  game_clock::setMultiplier(2);

  command_line commandLine { cmdLine };

  if( commandLine.Contains(L"-u") )
  {
    swap_chain::unlockFrameRate();
  }

  if( !commandLine.Contains(L"-w") )
  {
    swap_chain::fullScreen();
  }

  render_screen<main_menu_screen> mainMenu { 60, DIK_F12 };
  windows_message_loop::run(mainMenu);

  renderer::destroy();
  audio_output::destroy();
  diagnostics::destroy();
  dwrite_factory::destroy();
  windows_message_loop::destroy();
  user_input::destroy();
  render_target::destroy();
  d2d_factory::destroy();
  swap_chain::destroy();
  main_window::destroy();

  return 0;
}
