#include "pch.h"

#include "framework.h"
#include "level_geometries.h"
#include "renderers.h"
#include "game_settings.h"
#include "game_clock.h"
#include "screen_container.h"
#include "main_menu_screen.h"

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

auto format(DXGI_SWAP_CHAIN_DESC& swapChainDesc) -> void;
auto create_d2d_render_target() -> void;
auto destroy_d2d_render_target() -> void;
auto create_input_devices(HINSTANCE instance) -> void;
auto destroy_input_devices() -> void;
auto create_directx_objects(HINSTANCE instance) -> void;
auto destroy_directx_objects() -> void;
auto RunMainMenuScreen() -> void;

auto APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) -> int
{
  command_line::create(cmdLine);
  
  log::create();
  log::write(log::type::info, "app started");

  pseudo_random_generator::seed(static_cast<unsigned int>(performance_counter::QueryValue()));
  game_settings::load();

  game_settings::setFramerate(command_line::contains(L"-u") ? std::nullopt : std::optional<int>(60));
  log::write(log::type::info, "framerate {}", game_settings::framerate() ? std::format("{}", *game_settings::framerate()) : "UNCAPPED");

  game_settings::setFullscreen(command_line::contains(L"-w") ? false : true);
  log::write(log::type::info, "app is {}", game_settings::fullscreen() ? "FULLSCREEN" : "WINDOWED");

  main_window::create(instance, cmdShow);
  windows_message_loop::create();

  create_directx_objects(instance);

  diagnostics::create();

  sound_data::create(direct_sound::get_raw(), L"data");

  game_volume_controller::create();

  auto savedEffectsVolume = game_settings::effectsVolume();
  auto savedMusicVolume = game_settings::musicVolume();

  game_volume_controller::setEffectsVolume(0);
  game_volume_controller::setMusicVolume(0);

  {
    log::write(log::type::info, "play a sound to avoid any glitches when the game starts");
    sound_buffer_player player { sound_data::get(sound_data::menu_theme) };
    player.Play();
  }

  game_volume_controller::setEffectsVolume(savedEffectsVolume);
  game_volume_controller::setMusicVolume(savedMusicVolume);

  game_clock::setMultiplier(1.6f);

  swap_chain::get()->SetFullscreenState(game_settings::fullscreen() ? TRUE : FALSE, nullptr);

  RunMainMenuScreen();

  log::write(log::type::info, "app closing");

  game_volume_controller::destroy();
  sound_data::destroy();
  diagnostics::destroy();

  destroy_directx_objects();

  windows_message_loop::destroy();
  main_window::destroy();
  log::destroy();
  command_line::destroy();

  return 0;
}

auto format(DXGI_SWAP_CHAIN_DESC& swapChainDesc) -> void
{
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = 1920;
  swapChainDesc.BufferDesc.Height = 1080;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  auto framerate = game_settings::framerate();
  swapChainDesc.BufferDesc.RefreshRate.Numerator = framerate ? *framerate : 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = main_window::handle();
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  // swapChainDesc.Windowed = game_settings::fullscreen() ? FALSE : TRUE;
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

auto create_directx_objects(HINSTANCE instance) -> void
{
  direct_sound::create(main_window::handle());
  primary_sound_buffer::create(direct_sound::get_raw());
  create_d2d_render_target();
  dwrite_factory::create();
  level_geometries::create();
  renderer::create();
  create_input_devices(instance);
}

auto destroy_directx_objects() -> void
{
  destroy_input_devices();
  renderer::destroy();
  level_geometries::destroy();
  dwrite_factory::destroy();
  destroy_d2d_render_target();
  primary_sound_buffer::destroy();
  direct_sound::destroy();
}

auto RunMainMenuScreen() -> void
{
  log::write(log::type::info, "opening main menu screen");
  screen_container<main_menu_screen> mainMenu { game_settings::framerate(), DIK_F12 };
  windows_message_loop::run(mainMenu);  
}
