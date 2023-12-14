#include "pch.h"

#include "framework.h"
#include "main_menu_screen.h"
#include "renderers.h"
#include "game_settings.h"
#include "game_clock.h"
#include "screen_container.h"
#include "log.h"

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

auto format(DXGI_SWAP_CHAIN_DESC& swapChainDesc) -> void
{
  auto framerate = game_settings::framerate();
  int screenRefreshRate = framerate ? *framerate : 60;

  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = 1920;
  swapChainDesc.BufferDesc.Height = 1080;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = screenRefreshRate;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = main_window::handle();
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = TRUE;
}

auto create_d2d_render_target() -> void
{
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  format(swapChainDesc);
  D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, };
  swap_chain::create(swapChainDesc, featureLevels);
  d2d_factory::create(); 
  render_target::create(swap_chain::get_raw(), d2d_factory::get_raw());
}

auto destroy_d2d_render_target() -> void
{
  render_target::destroy();
  d2d_factory::destroy();
  swap_chain::destroy();
}

auto create_input_devices(HINSTANCE instance) -> void
{
  direct_input::create(instance);
  keyboard_device::create(direct_input::get_raw(), main_window::handle());
  keyboard_reader::create(keyboard_device::get());
  gamepad_reader::create();
}

auto destroy_input_devices() -> void
{
  gamepad_reader::destroy();
  keyboard_reader::destroy();
  keyboard_device::destroy();
  direct_input::destroy();
}

auto create_all(HINSTANCE instance) -> void
{
#ifndef STEAMDECK_BUILD
  create_d2d_render_target();
  dwrite_factory::create();
#endif
  create_input_devices(instance);
  diagnostics::create();
#ifndef STEAMDECK_BUILD
  direct_sound::create(main_window::handle());
  primary_sound_buffer::create(direct_sound::get_raw());
  renderer::create();
  sound_data::create(direct_sound::get_raw(), L"data");
  game_volume_controller::create();
#endif
}

auto destroy_all() -> void
{
#ifndef STEAMDECK_BUILD
  game_volume_controller::destroy();
  sound_data::destroy();
  renderer::destroy();
  primary_sound_buffer::destroy();
  direct_sound::destroy();
#endif
  diagnostics::destroy();
  destroy_input_devices();
#ifndef STEAMDECK_BUILD
  dwrite_factory::destroy();
  destroy_d2d_render_target();
#endif
}

auto APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) -> int
{
  command_line::create(cmdLine);
  
  log::create();
  log::open();
  log::file() << "log file opened\n";

  pseudo_random_generator::seed(static_cast<unsigned int>(performance_counter::QueryValue()));
  game_settings::load();

  if( command_line::contains(L"-u") )
  {
    game_settings::setFramerate(std::nullopt);
  }

  main_window::create(instance, cmdShow);
  windows_message_loop::create();

  create_all(instance);

#ifndef STEAMDECK_BUILD
  BOOL fullscreen = command_line::contains(L"-w") ? FALSE : TRUE;
  swap_chain::get()->SetFullscreenState(fullscreen, NULL);

  game_volume_controller::setEffectsVolume(6);
  game_volume_controller::setMusicVolume(7);

  // play sound now to ensure no sound glitch on first real play
  {
    sound_buffer_player player { sound_data::get(sound_data::menu_theme) };
    player.Play();
  }
#endif

  game_clock::setMultiplier(1.6f);

#ifndef STEAMDECK_BUILD
  screen_container<main_menu_screen> mainMenu { game_settings::framerate(), DIK_F12 };
  windows_message_loop::run(mainMenu);
#else
  windows_message_loop::run( []() -> bool
  {
    keyboard_reader::update();
    return keyboard_reader::pressed(DIK_ESCAPE) ? false : true;
  });
#endif
  
  destroy_all();

  windows_message_loop::destroy();
  main_window::destroy();
  log::file() << "log file closing\n";
  log::destroy();
  command_line::destroy();

  return 0;
}
