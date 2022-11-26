#include "pch.h"

#include "framework.h"
#include "math.h"
#include "render.h"
#include "control_state.h"
#include "main_menu_screen.h"
#include "play_screen.h"
#include "level_edit_screen.h"
#include "global_state.h"
#include "screens.h"

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

screen_id RunMainMenuScreen(d2d_app& app, global_state& globalState);
bool ProcessMessage();

extern const int fps = 60;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");
  const auto& dataPath = configFile.settings[L"data_path"];

  d2d_app app(hInstance, nCmdShow, fps);

  global_state globalState(app, dataPath);
  
  HRESULT hr = app.dxgi_swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) return 0;

  while( ProcessMessage() )
  {
    switch( globalState.currentScreenId )
    {
    case screen_main_menu:
      RunScreen<global_state, main_menu_screen_state, main_menu_control_state>(app, globalState);
      break;
    case screen_play:
      RunScreen<global_state, play_screen_state, play_screen_control_state>(app, globalState);
      break;
    case screen_level_editor:
      RunScreen<global_state, level_edit_screen_state, level_edit_control_state>(app, globalState);
      break;
    }

    if( globalState.currentScreenId == screen_id::screen_none ) ::PostQuitMessage(0);
  }

  return 0;
}
