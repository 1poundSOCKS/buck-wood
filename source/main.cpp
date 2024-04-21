#include "pch.h"

#include "framework.h"
#include "level_geometries.h"
#include "renderers.h"
#include "game_settings.h"
#include "game_clock.h"
#include "screen_container.h"
#include "main_menu_screen.h"
#include "audio_data.h"
#include "play_events.h"
#include "save_data.h"
#include "game_state.h"
#include "game_level_data_loader.h"
#include "player_state.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib,"Dwrite")
#pragma comment(lib,"Dinput8.lib")
#pragma comment(lib,"Xinput.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"RuntimeObject.lib")
#pragma comment(lib,"Shell32.lib")

auto initialize_all(HINSTANCE instance) -> void;
auto destroy_all() -> void;
auto run_main_menu_screen() -> void;
auto format(DXGI_SWAP_CHAIN_DESC& swapChainDesc) -> void;

auto APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) -> int
{
  command_line::create(cmdLine);
  
  log::create();
  log::write(log::type::info, "app started");

  pseudo_random_generator::seed(static_cast<unsigned int>(performance_counter::QueryValue()));

  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  DEVMODEA deviceMode;
  DWORD refreshRate = EnumDisplaySettingsA(nullptr, ENUM_CURRENT_SETTINGS, &deviceMode) ? deviceMode.dmDisplayFrequency : 0;

  game_settings::create();
  game_settings::load();
  game_settings::setRenderTargetWidth(screenWidth);
  game_settings::setRenderTargetHeight(screenHeight);
  game_settings::setSwapChainRefreshRate(refreshRate);
  command_line::contains(L"-u") ? game_settings::setFramerateUncapped() : game_settings::setFramerateCapped();
  game_settings::setFullscreen(command_line::contains(L"-w") ? false : true);
  game_settings::setShowDiagnostics(command_line::contains(L"-d"));
  game_settings::setCollisionDetectionType(command_line::contains(L"-cb") ? game_settings::collision_detection_type::basic : game_settings::collision_detection_type::direct2d);

  log::write(log::type::info, "framerate {}", game_settings::framerateCapped() ? std::format("{}", game_settings::swapChainRefreshRate()) : "UNCAPPED");
  log::write(log::type::info, "render target width {}", game_settings::renderTargetWidth());
  log::write(log::type::info, "render target height {}", game_settings::renderTargetHeight());
  log::write(log::type::info, "app is {}", game_settings::fullscreen() ? "FULLSCREEN" : "WINDOWED");
  log::write(log::type::info, "diagnostics {}", game_settings::showDiagnostics() ? "ON" : "OFF");

  main_window::create(instance, cmdShow);
  windows_message_loop::create();

  initialize_all(instance);

  game_clock::setMultiplier(1.6f);

  run_main_menu_screen();

  log::write(log::type::info, "app closing");

  destroy_all();

  windows_message_loop::destroy();
  main_window::destroy();
  game_settings::destroy();
  log::destroy();
  command_line::destroy();

  return 0;
}

auto initialize_all(HINSTANCE instance) -> void
{
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  format(swapChainDesc);
  D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, };
  swap_chain::create(swapChainDesc, featureLevels);
  d2d_factory::create(); 
  render_target::create(swap_chain::get_raw(), d2d_factory::get_raw());
  dwrite_factory::create();

  xaudio2_engine::create();
  xaudio2_masteringvoice::create(xaudio2_engine::get_raw());

  level_geometries::create();
  renderer::create();
  diagnostics::create();

  direct_input::create(instance);
  keyboard_device::create(direct_input::get_raw(), main_window::handle());
  keyboard_reader::create(keyboard_device::get());
  gamepad_reader::create();
  gamepad_reader::invert_y_axis();

  audio_data::create(L"data");
  audio_events::create();

  game_volume_controller::create();
  game_volume_controller::setEffectsVolume(game_settings::effectsVolume());
  game_volume_controller::setMusicVolume(game_settings::musicVolume());

  save_data::create(L"save_data");
  game_state::create();
  game_level_data_loader::create();
  play_events::create();
  player_state::create();

  save_data::read(game_state::get());
}

auto destroy_all() -> void
{
  player_state::destroy();
  play_events::destroy();
  game_level_data_loader::destroy();
  game_state::destroy();
  save_data::destroy();
  
  game_volume_controller::destroy();
  audio_events::destroy();

  gamepad_reader::destroy();
  keyboard_reader::destroy();
  keyboard_device::destroy();
  direct_input::destroy();

  diagnostics::destroy();
  renderer::destroy();
  level_geometries::destroy();
  xaudio2_masteringvoice::destroy();
  xaudio2_engine::destroy();

  audio_data::destroy();

  dwrite_factory::destroy();
  render_target::destroy();
  d2d_factory::destroy();
  swap_chain::destroy();
}

auto run_main_menu_screen() -> void
{
  log::write(log::type::info, "opening main menu screen");
  screen_container<main_menu_screen> mainMenu { game_settings::framerateCapped(), DIK_F12 };
  windows_message_loop::run(mainMenu);
}

auto format(DXGI_SWAP_CHAIN_DESC& swapChainDesc) -> void
{
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = game_settings::renderTargetWidth();
  swapChainDesc.BufferDesc.Height = game_settings::renderTargetHeight();
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = game_settings::swapChainRefreshRate();
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = main_window::handle();
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = game_settings::fullscreen() ? FALSE : TRUE;
}
