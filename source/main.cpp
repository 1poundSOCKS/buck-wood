#include "pch.h"

#include "framework.h"
#include "global_state.h"
#include "screen_runner.h"
#include "main_window.h"
#include "screen_render_data.h"
#include "sound_data.h"

#include "main_menu_screen_state.h"
#include "play_screen_state.h"

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

const int fps = 60;

void UpdateGlobalState(global_state& globalState, const play_screen_state& screenState);
// void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState);

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");
  const auto& dataPath = configFile.settings[L"data_path"];

  framework::create(instance, cmdShow);

  sound_buffers soundBuffers = LoadSoundBuffers(framework::directSound().get(), dataPath);

  // play sound now to ensure no sound glitch on first real play
  {
    global_sound_buffer_selector dummySelector { soundBuffers };
    sound_buffer_player dummyPlayer(dummySelector[menu_theme]);
    dummyPlayer.Play();
  }

  global_state globalState(dataPath);
  
  HRESULT hr = framework::swapChain()->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) return 0;

  screen_render_data screenRenderData {
    CreateScreenRenderBrushes(framework::renderTarget().get()), 
    CreateScreenRenderTextFormats(framework::dwriteFactory().get())
  };
  
  sound_data soundData { soundBuffers };

  bool continueRunning = true;
  bool saveGameLevelData = false;

  while( continueRunning )
  {
    main_menu_screen_state mainMenuScreenState(screenRenderData);
    framework::openScreen(mainMenuScreenState);

    if( mainMenuScreenState.StartPlay() )
    {
      play_screen_state playScreenState(
        globalState.gameLevelDataIndex->gameLevelData.cbegin(), 
        globalState.gameLevelDataIndex->gameLevelData.cend(),
        screenRenderData,
        soundData
      );
      
      framework::openScreen(playScreenState);
    }
    // else if( mainMenuScreenState.startLevelEdit )
    // {
    //   level_edit_screen_state levelEditScreenState(*globalState.gameLevelDataIndex, screenRenderData);
      
    //   OpenScreen(screenRunnerData, levelEditScreenState);
      
    //   if( levelEditScreenState.saveChanges )
    //   {
    //     UpdateGlobalState(globalState, levelEditScreenState);
    //     mainMenuScreenState.checkSaveOnExit = true;
    //   }
    // }
    else
    {
      continueRunning = false;
      saveGameLevelData = mainMenuScreenState.SaveGameLevelData();
    }
  }

  if( saveGameLevelData )
    SaveAllGameLevelData(*globalState.gameLevelDataIndex);

  return 0;
}

// void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
// {
//   globalState.gameLevelDataIndex = std::make_unique<game_level_data_index>(screenState.gameLevelDataIndex);
//   globalState.gameLevelDataIndexUpdated = true;
//   globalState.currentScreenId = screen_main_menu;
// }
