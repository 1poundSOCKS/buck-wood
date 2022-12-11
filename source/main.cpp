#include "pch.h"

#include "framework.h"
#include "math.h"
#include "render.h"
#include "level_edit_screen.h"
#include "play_screen.h"
#include "main_menu_screen.h"
#include "global_state.h"
#include "screen_runner.h"
#include "main_window.h"
#include "screen_render_data.h"
#include "screen_sound_data.h"

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

void UpdateGlobalState(global_state& globalState, const play_screen_state& screenState);
void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState);

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
  auto dwriteFactory = CreateDWriteFactory();
  auto directSound = CreateDirectSound(window);
  auto primarySoundBuffer = CreatePrimarySoundBuffer(directSound.get());
  auto keyboard = CreateKeyboard(hInstance, window);

  screen_render_brushes renderBrushes;
  CreateScreenRenderBrushes(renderTarget.get(), renderBrushes);
  screen_render_brush_selector renderBrushSelector { renderBrushes };

  screen_render_text_formats textFormats;
  CreateScreenRenderTextFormats(dwriteFactory.get(), textFormats);
  screen_render_text_format_selector textFormatSelector { textFormats};

  sound_buffers soundBuffers;
  LoadSoundBuffers(directSound.get(), dataPath, soundBuffers);
  global_sound_buffer_selector soundBufferSelector(soundBuffers);

  // ensure no sound glitch on first play
  {
    global_sound_buffer_selector dummySelector { soundBuffers };
    sound_buffer_player dummyPlayer(dummySelector[menu_theme]);
    dummyPlayer.Play();
  }

  global_state globalState(dataPath);
  
  HRESULT hr = swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) return 0;

  system_timer systemTimer;
  perf_data perfData;

  screen_runner_data screenRunnerData
  {
    swapChain,
    renderTarget, 
    dwriteFactory,
    keyboard, 
    windowData, 
    systemTimer, 
    perfData, 
  };

  bespoke_render_data bespokeRenderData { renderBrushes, textFormats };

  bespoke_sound_data bespokeSoundData { soundBuffers };

  main_menu_screen_state mainMenuScreenState;

  while( !mainMenuScreenState.quit )
  {
    Start(screenRunnerData, bespokeRenderData, bespokeSoundData, mainMenuScreenState);

    if( mainMenuScreenState.startPlay )
    {
      play_screen_state playScreenState(
        globalState.gameLevelDataIndex->gameLevelData.cbegin(), 
        globalState.gameLevelDataIndex->gameLevelData.cend()
      );
      
      Start(screenRunnerData, bespokeRenderData, bespokeSoundData, playScreenState);
      
      UpdateGlobalState(globalState, playScreenState);
    }
    else if( mainMenuScreenState.startLevelEdit )
    {
      level_edit_screen_state levelEditScreenState(*globalState.gameLevelDataIndex);
      
      Start(screenRunnerData, bespokeRenderData, bespokeSoundData, levelEditScreenState);
      
      if( levelEditScreenState.saveChanges )
      {
        UpdateGlobalState(globalState, levelEditScreenState);
        mainMenuScreenState.checkSaveOnExit = true;
      }
    }
    else
      mainMenuScreenState.quit = true;

    mainMenuScreenState.startPlay = mainMenuScreenState.startLevelEdit = false;
  }

  if( mainMenuScreenState.saveGameLevelData )
    SaveAllGameLevelData(*globalState.gameLevelDataIndex);

  return 0;
}

void UpdateGlobalState(global_state& globalState, const play_screen_state& screenState)
{
  globalState.currentScreenId = screen_main_menu;
}

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
{
  globalState.gameLevelDataIndex = std::make_unique<game_level_data_index>(screenState.gameLevelDataIndex);
  globalState.gameLevelDataIndexUpdated = true;
  globalState.currentScreenId = screen_main_menu;
}
