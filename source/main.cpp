#include "pch.h"

#include "framework.h"
#include "math.h"
#include "render.h"
#include "control_state.h"
#include "main_menu/main_menu_screen.h"
#include "play/play_screen_state.h"
#include "play/play_screen_render.h"
#include "play/play_screen_sound.h"
#include "level_edit/level_edit_screen.h"
#include "global_state.h"
#include "screens.h"
#include "main_window.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib,"Dwrite")
#pragma comment(lib,"Dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")
#pragma comment(lib,"RuntimeObject.lib")
#pragma comment(lib,"jsoncpp.lib")

extern const int fps = 60;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");
  const auto& dataPath = configFile.settings[L"data_path"];

  RegisterMainWindowClass(hInstance);
  window_data windowData;
  auto window = CreateMainWindow(hInstance, nCmdShow, windowData);
  auto swapChain = CreateSwapChain(window, 60, 1);
  auto renderTarget = CreateRenderTarget(swapChain.get());
  auto directSound = CreateDirectSound(window);
  auto primarySoundBuffer = CreatePrimarySoundBuffer(directSound.get());
  auto keyboard = CreateKeyboard(hInstance, window);

  sound_buffers soundBuffers;
  LoadSoundBuffers(directSound.get(), dataPath, soundBuffers);
  global_sound_buffer_selector_type soundBufferSelector(soundBuffers);

  global_state globalState(dataPath);
  
  HRESULT hr = swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) return 0;

  system_timer timer;
  perf_data perfData;

  while( ProcessMessage() )
  {
    switch( globalState.currentScreenId )
    {
    case screen_main_menu:
      RunScreen<global_state, main_menu_screen_state, main_menu_control_state, global_sound_buffer_selector_type>
      (swapChain.get(), renderTarget.get(), keyboard.get(), windowData, timer, perfData, globalState, soundBufferSelector);
      break;
    case screen_play:
      RunScreen<global_state, play_screen_state, play_screen_control_state>
      (swapChain.get(), renderTarget.get(), keyboard.get(), windowData, timer, perfData, globalState, soundBufferSelector);
      break;
    case screen_level_editor:
      RunScreen<global_state, level_edit_screen_state, level_edit_control_state>
      (swapChain.get(), renderTarget.get(), keyboard.get(), windowData, timer, perfData, globalState, soundBufferSelector);
      break;
    }

    if( globalState.currentScreenId == screen_id::screen_none ) ::PostQuitMessage(0);
  }

  return 0;
}
