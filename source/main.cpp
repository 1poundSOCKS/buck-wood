#include "pch.h"

#include "framework.h"
#include "global_state.h"
#include "screen_runner.h"
#include "main_window.h"
#include "sound_data.h"

#include "main_menu_screen.h"
#include "play_screen.h"

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

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file::create(L"config.txt");
  const auto& dataPath = config_file::getSetting(L"data_path");
  framework::create(instance, cmdShow);
  global_state::create(dataPath);
  sound_data::create(framework::directSound().get(), dataPath);

  // play sound now to ensure no sound glitch on first real play
  {
    global_sound_buffer_selector dummySelector { sound_data::soundBuffers() };
    sound_buffer_player dummyPlayer(dummySelector[menu_theme]);
    dummyPlayer.Play();
  }

  main_menu_screen mainMenuScreen;
  framework::openScreen(mainMenuScreen);

  if( mainMenuScreen.SaveGameLevelData() )
    global_state::save();

  return 0;
}
