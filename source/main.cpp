#include "pch.h"

#include "framework.h"
#include "math.h"
#include "render.h"
#include "control_state.h"
#include "main_menu_screen.h"
#include "play_screen.h"
#include "level_edit_screen.h"
#include "global_state.h"

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

enum screen_type { screen_none, screen_main_menu, screen_play, screen_level_editor };

template<class T_SS, class T_CS> screen_type RunScreen(d2d_app& app, global_state& globalState);
template<class T_SS, class T_CS> void UpdateScreen(d2d_app& app, const global_state& globalState, T_SS& screenState);
screen_type RunMainMenuScreen(d2d_app& app, global_state& globalState);
screen_type RunPlayScreen(d2d_app& app, global_state& globalState);
screen_type RunLevelEditorScreen(d2d_app& app, global_state& globalState);
bool ProcessMessage();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");
  const auto& dataPath = configFile.settings[L"data_path"];

  static const int fps = 60;

  d2d_app app(hInstance, nCmdShow, fps);

  global_state globalState(app, dataPath);
  
  HRESULT hr = app.dxgi_swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) return 0;

  screen_type currentScreen = screen_main_menu;

  while( ProcessMessage() )
  {
    switch( currentScreen )
    {
    case screen_main_menu:
      currentScreen = RunMainMenuScreen(app, globalState);
      break;
    case screen_play:
      currentScreen = RunScreen<play_screen_state, play_screen_control_state>(app, globalState);
      break;
    case screen_level_editor:
      currentScreen = RunScreen<level_edit_screen_state, level_edit_control_state>(app, globalState);
      break;
    }
  }

  return 0;
}

screen_type RunMainMenuScreen(d2d_app& app, global_state& globalState)
{
  main_menu_screen_state screenState = main_menu_screen_state(globalState);

  while( ProcessMessage() )
  {
    UpdateScreen<main_menu_screen_state, main_menu_control_state>(app, globalState, screenState);

    if( screenState.quit )
    {
      UpdateGlobalState(globalState, screenState);
      if( globalState.saveGameLevelData ) SaveAllGameLevelData(*globalState.gameLevelDataIndex);
      ::PostQuitMessage(0);
      return screen_none;
    }

    if( screenState.startPlay )
      return screen_play;
    
    if( screenState.startLevelEdit )
      return screen_level_editor;
	}

  return screen_none;
}

template<class T_SS, class T_CS>
screen_type RunScreen(d2d_app& app, global_state& globalState)
{
  T_SS screenState(app, globalState);

  while( ProcessMessage() )
  {
    UpdateScreen<T_SS, T_CS>(app, globalState, screenState);
    if( screenState.returnToMenu )
    {
      UpdateGlobalState(globalState, screenState);
      return screen_main_menu;
    }
  }

  return screen_none;
}

template<class T_SS, class T_CS>
void UpdateScreen(d2d_app& app, const global_state& globalState, T_SS& screenState)
{
  static input_state inputState, previousInputState;
  previousInputState = inputState;
  inputState.RefreshKeyboard(app.keyboard);
  inputState.clientMouseData = app.clientMouseData;
  inputState.renderTargetMouseData = app.renderTargetMouseData;
  inputState.leftMouseButtonDown = app.leftMouseButtonDown;
  inputState.rightMouseButtonDown = app.rightMouseButtonDown;

  static control_state baseControlState;
  baseControlState.Refresh(inputState, previousInputState);

  static T_CS screenControlState;
  RefreshControlState(screenControlState, baseControlState);

  auto start = QueryPerformanceCounter();
  UpdateScreenState(screenState, screenControlState, *app.timer);
  auto end = QueryPerformanceCounter();
  app.perfData->updateScreenStateTicks = end - start;
  
  static diagnostics_data diagnosticsData;
  diagnosticsData.clear();
  diagnosticsData.reserve(50);
  FormatDiagnostics(std::back_inserter(diagnosticsData), *app.perfData, *app.timer);
  FormatDiagnostics(std::back_inserter(diagnosticsData), globalState);
  FormatDiagnostics(std::back_inserter(diagnosticsData), baseControlState);
  FormatDiagnostics(std::back_inserter(diagnosticsData), screenState, screenControlState);

  if( baseControlState.f12Press ) app.dxgi_swapChain->SetFullscreenState(TRUE, NULL);

  {
    d2d_frame frame(app.d2d_rendertarget);
    auto start = QueryPerformanceCounter();
    RenderFrame(frame, screenState);
    auto end = QueryPerformanceCounter();
    app.perfData->renderFrameTicks = end - start;
    RenderDiagnostics(frame.renderTarget, diagnosticsData, globalState.textFormats, globalState.renderBrushes.brushGrey);
    frame.renderTarget->SetTransform(D2D1::IdentityMatrix());
    RenderMouseCursor(frame.renderTarget, baseControlState.renderTargetMouseData.x, baseControlState.renderTargetMouseData.y, globalState.renderBrushes);
  }

  app.dxgi_swapChain->Present(1, 0);

  PlaySoundEffects(screenState);
  
  UpdateTimer(*app.timer);
  UpdatePerformanceData(*app.perfData);
}

bool ProcessMessage()
{
  MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return (msg.message != WM_QUIT);
	}

  return true;
}
