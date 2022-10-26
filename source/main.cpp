#define UNICODE
#define USE_JSON

#include <iostream>
#include <tchar.h>
#include <math.h>
#include <filesystem>

#include "type_defs.h"
#include "math.h"
#include "d2d_app.h"
#include "render.h"
#include "control_state.h"
#include "data_files.h"
#include "main_menu_screen.h"
#include "play_screen.h"
#include "level_edit_screen.h"

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

namespace fs = std::filesystem;

#ifdef USE_JSON
#pragma comment(lib,"jsoncpp.lib")
#endif

template<class T> void UpdateScreen(d2d_app& app, T& screenState, sound_buffers& soundBuffers);
void RunPlayScreen(d2d_app& app, sound_buffers& soundBuffers, const game_level_data_index& gameLevelDataIndex);
void RunLevelEditorScreen(d2d_app& app, sound_buffers& soundBuffers, const game_level_data_index& gameLevelDataIndex);
bool ProcessMessage(MSG* msg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");

  static const int fps = 60;

  auto app = std::make_unique<d2d_app>(hInstance, nCmdShow, fps);
  
  const std::wstring dataPath = configFile.settings[L"data_path"];

  auto gameLevelDataIndexPtr = LoadAllGameLevelData(dataPath);

  auto soundBuffers = std::make_unique<sound_buffers>(app->directSound, dataPath);

  HRESULT hr = S_OK;

  hr = app->dxgi_swapChain->SetFullscreenState(TRUE, NULL);
  if( FAILED(hr) ) return 0;

  auto menuScreenState = std::make_unique<main_menu_screen_state>();

  MSG msg;
  while (ProcessMessage(&msg))
  {
    if( app->terminating ) continue;

    UpdateScreen(*app, *menuScreenState, *soundBuffers);

    if( menuScreenState->quit )
    {
      ::PostQuitMessage(0);
      app->terminating = true;
      continue;
    }

    if( menuScreenState->startPlay ) RunPlayScreen(*app, *soundBuffers, *gameLevelDataIndexPtr);

    else if( menuScreenState->startLevelEdit )
    {
    }
	}

  return (int) msg.wParam;
}

void RunPlayScreen(d2d_app& app, sound_buffers& soundBuffers, const game_level_data_index& gameLevelDataIndex)
{
  MSG msg;
  play_screen_state playScreenState(*app.timer, gameLevelDataIndex);
  
  while (ProcessMessage(&msg) && !playScreenState.returnToMenu )
  {
    UpdateScreen(app, playScreenState, soundBuffers);
  }
}

void RunLevelEditorScreen(d2d_app& app, sound_buffers& soundBuffers, const game_level_data_index& gameLevelDataIndex)
{
  MSG msg;
  level_edit_screen_state levelEditScreenState(gameLevelDataIndex);

  while (ProcessMessage(&msg) && !levelEditScreenState.returnToMenu )
  {
    UpdateScreen(app, levelEditScreenState, soundBuffers);
  }
}

template<class T> void UpdateScreen(d2d_app& app, T& screenState, sound_buffers& soundBuffers)
{
  auto frame = std::make_unique<d2d_frame>(app.d2d_rendertarget, app.brushes, app.textFormats);
  frame->renderTargetMouseX = app.previousControlState->renderTargetMouseX;
  frame->renderTargetMouseY = app.previousControlState->renderTargetMouseY;

  RenderFrame(*frame, screenState);

  app.mouseCursor->xPos = app.previousControlState->renderTargetMouseX;
  app.mouseCursor->yPos = app.previousControlState->renderTargetMouseY;
  RenderMouseCursor(*frame, *app.mouseCursor);

  app.dxgi_swapChain->Present(1, 0);

  app.controlState = GetControlState(app, *app.previousControlState);

  UpdatePerformanceData(*app.perfData);

  static diagnostics_data diagnosticsData;
  diagnosticsData.clear();
  diagnosticsData.reserve(20);
  FormatDiagnostics(diagnosticsData, screenState, *app.controlState, *app.perfData, *app.timer);
  RenderDiagnostics(*frame, diagnosticsData);

  UpdateState(screenState, *app.controlState, *app.timer);

  UpdateTimer(*app.timer);

  UpdateSound(soundBuffers, screenState);

  app.previousControlState = std::move(app.controlState);
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
