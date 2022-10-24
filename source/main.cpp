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

template<class T> void RenderFrameAndUpdateState(d2d_app& app, T& gameState, sound_buffers& soundBuffers);
bool ProcessMessage(MSG* msg);
void FormatDiagnostics(std::list<std::wstring>& diagnostics, const game_state& gameState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);
void FormatDiagnostics(std::list<std::wstring>& diagnostics, const play_state& playState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);
void FormatDiagnostics(std::list<std::wstring>& diagnostics, const control_state& controlState, const perf_data& perfData, const system_timer& timer);
game_level_data_ptr LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file);
std::wstring GetFullLevelFilename(const std::wstring& dataPath, const std::wstring& file);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");

  static const int fps = 60;

  const std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance, nCmdShow, fps);
  
  const std::wstring dataPath = configFile.settings[L"data_path"];

  game_data_ptr gameDataPtr = std::make_shared<game_data>();
  gameDataPtr->push_back(LoadGameLevelData(dataPath, L"level_001.json"));
  gameDataPtr->push_back(LoadGameLevelData(dataPath, L"level_002.json"));
  gameDataPtr->push_back(LoadGameLevelData(dataPath, L"level_003.json"));
  gameDataPtr->push_back(LoadGameLevelData(dataPath, L"level_004.json"));

  sound_buffers_ptr soundBuffers = std::make_unique<sound_buffers>(app->directSound, dataPath);

  HRESULT hr = S_OK;

  hr = app->dxgi_swapChain->SetFullscreenState(TRUE, NULL);
  if( FAILED(hr) ) return 0;

  game_state_ptr gameStatePtr = std::make_unique<game_state>();
  play_state_ptr playStatePtr = std::make_unique<play_state>(*app->timer, gameDataPtr);

  screen_type currentScreen = screen_menu;

  MSG msg;
  while (ProcessMessage(&msg))
  {
    if( app->terminating ) continue;

    switch( currentScreen )
    {
    case screen_menu:
      RenderFrameAndUpdateState(*app, *gameStatePtr, *soundBuffers);
      if( gameStatePtr->startPlay )
      {
        playStatePtr = std::make_unique<play_state>(*app->timer, gameDataPtr);
        currentScreen = screen_play;
      }
      else if( gameStatePtr->quit )
      {
        ::PostQuitMessage(0);
        app->terminating = true;
      }
      break;
    case screen_play:
      RenderFrameAndUpdateState(*app, *playStatePtr, *soundBuffers);
      if( playStatePtr->returnToMenu )
      {
        gameStatePtr->startPlay = false;
        currentScreen = screen_menu;
      }
      break;
    }
	}

  return (int) msg.wParam;
}

template<class T> void RenderFrameAndUpdateState(d2d_app& app, T& state, sound_buffers& soundBuffers)
{
  std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app.d2d_rendertarget, app.brushes, app.textFormats);
  frame->renderTargetMouseX = app.previousControlState->renderTargetMouseX;
  frame->renderTargetMouseY = app.previousControlState->renderTargetMouseY;

  RenderFrame(*frame, state);

  app.mouseCursor->xPos = app.previousControlState->renderTargetMouseX;
  app.mouseCursor->yPos = app.previousControlState->renderTargetMouseY;
  RenderMouseCursor(*frame, *app.mouseCursor);

  app.dxgi_swapChain->Present(1, 0);

  app.controlState = GetControlState(app, *app.previousControlState);

  UpdatePerformanceData(*app.perfData);

  std::list<std::wstring> diagnostics;
  FormatDiagnostics(diagnostics, state, *app.controlState, *app.perfData, *app.timer);
  RenderDiagnostics(*frame, diagnostics);

  UpdateState(state, *app.controlState, *app.timer);

  UpdateSystemTimer(*app.timer);

  UpdateSound(soundBuffers, state);

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

void FormatDiagnostics(std::list<std::wstring>& diagnostics, const game_state& gameState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnostics, controlState, perfData, timer);
}

void FormatDiagnostics(std::list<std::wstring>& diagnostics, const play_state& playState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnostics, controlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"bullet count: %I64u", playState.bullets.size());
  diagnostics.push_back(text);
}

void FormatDiagnostics(std::list<std::wstring>& diagnostics, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  static wchar_t text[64];

  float runTime = GetTotalTimeInSeconds(timer);
  float intervalTime = GetIntervalTimeInSeconds(timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnostics.push_back(text);

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnostics.push_back(text);

  swprintf(text, L"mouse x: %i", static_cast<int>(controlState.renderTargetMouseX));
  diagnostics.push_back(text);

  swprintf(text, L"mouse y: %i", static_cast<int>(controlState.renderTargetMouseY));
  diagnostics.push_back(text);
}
