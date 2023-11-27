#include "pch.h"

#include "framework.h"
#include "main_menu_screen.h"
#include "renderers.h"
#include "game_settings.h"
#include "game_clock.h"
#include "screen_container.h"

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

auto create_all(HINSTANCE instance, int cmdShow, int screenRefreshRate) -> void
{
  main_window::create(instance, cmdShow);
  windows_message_loop::create();
  swap_chain::create(main_window::handle(), screenRefreshRate, 1);
  d2d_factory::create(); 
  render_target::create(swap_chain::get_raw(), d2d_factory::get_raw());
  dwrite_factory::create();
  direct_input::create(instance);
  keyboard_device::create(direct_input::get_raw(), main_window::handle());
  keyboard_reader2::create(keyboard_device::get());
  user_input::create(keyboard_device::get());
  diagnostics::create();
  audio_output::create(main_window::handle());
  renderer::create();
  sound_data::create(L"data");
  game_volume_controller::create();
}

auto destroy_all() -> void
{
  game_volume_controller::destroy();
  sound_data::destroy();
  renderer::destroy();
  audio_output::destroy();
  diagnostics::destroy();
  user_input::destroy();
  keyboard_reader2::destroy();
  keyboard_device::destroy();
  direct_input::destroy();
  dwrite_factory::destroy();
  render_target::destroy();
  d2d_factory::destroy();
  swap_chain::destroy();
  windows_message_loop::destroy();
  main_window::destroy();
}

auto APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) -> int
{
  command_line::create(cmdLine);
  pseudo_random_generator::seed(static_cast<unsigned int>(performance_counter::QueryValue()));

  game_settings::load();

  if( command_line::contains(L"-u") )
  {
    game_settings::setFramerate(std::nullopt);
  }

  auto framerate = game_settings::framerate();
  int screenRefreshRate = framerate ? *framerate : 60;

  create_all(instance, cmdShow, screenRefreshRate);

  BOOL fullscreen = command_line::contains(L"-w") ? FALSE : TRUE;
  swap_chain::get()->SetFullscreenState(fullscreen, NULL);

  game_volume_controller::setEffectsVolume(6);
  game_volume_controller::setMusicVolume(7);

  // play sound now to ensure no sound glitch on first real play
  {
    sound_buffer_player player { sound_data::get(sound_data::menu_theme) };
    player.Play();
  }

  game_clock::setMultiplier(2);

  screen_container<main_menu_screen> mainMenu { game_settings::framerate(), DIK_F12 };
  windows_message_loop::run(mainMenu);

  destroy_all();

  command_line::destroy();

  return 0;
}
