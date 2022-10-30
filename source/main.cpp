#define UNICODE
#define USE_JSON

#include <iostream>
#include <tchar.h>
#include <math.h>
#include <filesystem>

#include "framework/framework.h"
#include "type_defs.h"
#include "math.h"
#include "render.h"
#include "control_state.h"
#include "main_menu_screen.h"
#include "play_screen.h"
#include "level_edit_screen.h"
#include "global_state.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")
#pragma comment(lib, "RuntimeObject.lib")

#ifdef USE_JSON
#pragma comment(lib,"jsoncpp.lib")
#endif

template<class T> void UpdateScreen(d2d_app& app, const global_state& globalState, T& screenState);
int RunMainMenuScreen(d2d_app& app, global_state& globalState);
void RunPlayScreen(d2d_app& app, global_state& globalState);
void RunLevelEditorScreen(d2d_app& app, global_state& globalState);
bool ProcessMessage(MSG* msg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");
  const auto& dataPath = configFile.settings[L"data_path"];

  static const int fps = 60;

  d2d_app app(hInstance, nCmdShow, fps);

  global_state globalState(app, dataPath);
  
  HRESULT hr = app.dxgi_swapChain->SetFullscreenState(TRUE, NULL);
  if( FAILED(hr) ) return 0;

  return RunMainMenuScreen(app, globalState);
}

int RunMainMenuScreen(d2d_app& app, global_state& globalState)
{
  main_menu_screen_state screenState = main_menu_screen_state();

  MSG msg;
  while (ProcessMessage(&msg))
  {
    if( app.terminating ) continue;

    UpdateScreen(app, globalState, screenState);

    if( screenState.quit )
    {
      ::PostQuitMessage(0);
      app.terminating = true;
      continue;
    }

    if( screenState.startPlay )
      RunPlayScreen(app, globalState);

    else if( screenState.startLevelEdit )
      RunLevelEditorScreen(app, globalState);
	}

  return (int) msg.wParam;
}

void RunPlayScreen(d2d_app& app, global_state& globalState)
{
  MSG msg;
  play_screen_state playScreenState(globalState, *app.timer);
  
  while (ProcessMessage(&msg) && !playScreenState.returnToMenu )
  {
    UpdateScreen(app, globalState, playScreenState);
  }
}

void RunLevelEditorScreen(d2d_app& app, global_state& globalState)
{
  MSG msg;
  level_edit_screen_state levelEditScreenState(globalState);

  while (ProcessMessage(&msg) && !levelEditScreenState.returnToMenu )
  {
    UpdateScreen(app, globalState, levelEditScreenState);
  }

  UpdateGlobalState(globalState, levelEditScreenState);
}

template<class T> void UpdateScreen(d2d_app& app, const global_state& globalState, T& screenState)
{
  static control_state controlState;
  
  const auto viewTransform = CreateViewTransform(app.d2d_rendertarget, screenState);
  
  RefreshInputState(app);
  RefreshControlState(controlState, app, viewTransform);

  UpdateScreenState(screenState, controlState, *app.timer);
  
  static diagnostics_data diagnosticsData;
  diagnosticsData.clear();
  diagnosticsData.reserve(20);
  FormatDiagnostics(diagnosticsData, screenState, controlState, *app.perfData, *app.timer);

  {
    d2d_frame frame(app.d2d_rendertarget, viewTransform, globalState.brushes, globalState.textFormats);
    frame.renderTargetMouseX = controlState.renderTargetMouseX;
    frame.renderTargetMouseY = controlState.renderTargetMouseY;
    RenderFrame(frame, screenState);
    RenderDiagnostics(frame, diagnosticsData);
  }

  app.dxgi_swapChain->Present(1, 0);

  UpdateSound(globalState.soundBuffers, screenState);

  UpdateTimer(*app.timer);
  UpdatePerformanceData(*app.perfData);
}

bool ProcessMessage(MSG* msg)
{
	if (PeekMessage(msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg->hwnd, NULL, msg))
    {
      TranslateMessage(msg);
      DispatchMessage(msg);
    }
    return (msg->message != WM_QUIT);
	}

  return true;
}
