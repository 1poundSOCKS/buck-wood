#include "pch.h"

#include "framework.h"
#include "math.h"
#include "render.h"
// #include "control_state.h"
// #include "main_menu/main_menu_screen.h"
// #include "play/play_screen_state.h"
// #include "play/play_screen_render.h"
// #include "play/play_screen_sound.h"
// #include "level_edit/level_edit_screen.h"
#include "play_screen.h"
#include "global_state.h"
#include "screen_runner.h"
#include "main_window.h"
// #include "control_state_reader.h"
#include "screen_render_data.h"

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

// void UpdateGlobalState(global_state& globalState, const main_menu_screen_state& screenState);
void UpdateGlobalState(global_state& globalState, const play_screen_state& screenState);
// void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState);

extern const int fps = 60;

using screen_runner_type = screen_runner
<
  screen_render_brush_selector, 
  screen_render_text_format_selector,
  global_sound_buffer_selector
>;

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
  auto dwriteFactory = CreateDWriteFactory();
  auto directSound = CreateDirectSound(window);
  auto primarySoundBuffer = CreatePrimarySoundBuffer(directSound.get());
  auto keyboard = CreateKeyboard(hInstance, window);
  // keyboard_state_reader keyboardStateReader { keyboard.get() };

  screen_render_brushes renderBrushes;
  CreateScreenRenderBrushes(renderTarget.get(), renderBrushes);
  screen_render_brush_selector renderBrushSelector { renderBrushes };

  screen_render_text_formats textFormats;
  CreateScreenRenderTextFormats(dwriteFactory.get(), textFormats);
  screen_render_text_format_selector textFormatSelector { textFormats};

  sound_buffers soundBuffers;
  LoadSoundBuffers(directSound.get(), dataPath, soundBuffers);
  global_sound_buffer_selector soundBufferSelector(soundBuffers);

  global_state globalState(dataPath);
  
  HRESULT hr = swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) return 0;

  system_timer systemTimer;
  perf_data perfData;

  // while( ProcessMessage() )
  // {
    // switch( globalState.currentScreenId )
    // {
    // case screen_main_menu:
    //   RunScreen<global_state, main_menu_screen_state, main_menu_control_state, global_sound_buffer_selector_type>
    //   (swapChain.get(), renderTarget.get(), keyboard.get(), windowData, timer, perfData, globalState, soundBufferSelector);
    //   break;
    // case screen_play:
    //   RunScreen<global_state, play_screen_state, play_screen_control_state>
    //   (swapChain.get(), renderTarget.get(), keyboard.get(), windowData, timer, perfData, globalState, soundBufferSelector);
    //   break;
    // case screen_level_editor:
    //   RunScreen<global_state, level_edit_screen_state, level_edit_control_state>
    //   (swapChain.get(), renderTarget.get(), keyboard.get(), windowData, timer, perfData, globalState, soundBufferSelector);
    //   break;
    // }

    // if( globalState.currentScreenId == screen_id::screen_none ) ::PostQuitMessage(0);
    // RunScreen<main_menu_screen_state, main_menu_control_state, global_sound_buffer_selector_type>(
    //   swapChain.get(), 
    //   renderTarget.get(), 
    //   keyboard.get(), 
    //   windowData, 
    //   systemTimer, 
    //   perfData, 
    //   soundBufferSelector, 
    //   *screenState);

  screen_runner_type screenRunner
  {
    swapChain.get(),
    renderTarget.get(), 
    dwriteFactory.get(),
    keyboard.get(), 
    windowData, 
    systemTimer, 
    perfData, 
    renderBrushSelector,
    textFormatSelector,
    soundBufferSelector
  };

  // {
  //   auto mainMenuScreenState = std::make_unique<main_menu_screen_state>();
  //   screenRunner.Start<main_menu_screen_state, main_menu_control_state>(*mainMenuScreenState);
  // }

  // {
  //   auto levelEditScreenState = std::make_unique<level_edit_screen_state>(*globalState.gameLevelDataIndex);
  //   screenRunner.Start<level_edit_screen_state, level_edit_control_state>(*levelEditScreenState);
  //   UpdateGlobalState(globalState, *levelEditScreenState);
  // }

  {
    // control_state_reader<play_screen_control_state> controlStateReader { renderTarget.get(), windowData, keyboard.get() };
    // play_screen_control_state_reader controlStateReader { windowData, keyboardStateReader };

    play_screen_state playScreenState(
      systemTimer, 
      globalState.gameLevelDataIndex->gameLevelData.cbegin(), 
      globalState.gameLevelDataIndex->gameLevelData.cend()
    );

    screenRunner.Start(playScreenState);

    UpdateGlobalState(globalState, playScreenState);
  }

  return 0;
}

// void UpdateGlobalState(global_state& globalState, const main_menu_screen_state& screenState)
// {
//   if( screenState.saveGameLevelData ) SaveAllGameLevelData(*globalState.gameLevelDataIndex);
  
//   if( screenState.startPlay ) globalState.currentScreenId = screen_play;
//   else if( screenState.startLevelEdit ) globalState.currentScreenId = screen_level_editor;
//   else if( screenState.quit ) globalState.currentScreenId = screen_none;
// }

void UpdateGlobalState(global_state& globalState, const play_screen_state& screenState)
{
  globalState.currentScreenId = screen_main_menu;
}

// void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
// {
//   if( screenState.saveChanges )
//   {
//     globalState.gameLevelDataIndex = std::make_unique<game_level_data_index>(screenState.gameLevelDataIndex);
//     globalState.gameLevelDataIndexUpdated = true;
//   }

//   globalState.currentScreenId = screen_main_menu;
// }
