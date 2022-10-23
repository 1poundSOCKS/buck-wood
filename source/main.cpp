#define UNICODE
#define USE_JSON

#include <iostream>
#include <tchar.h>
#include <math.h>
#include <filesystem>

#include "math.h"
#include "d2d_app.h"
#include "render.h"
#include "control_state.h"
#include "game_level.h"
#include "data_files.h"
#include "game_sound.h"
#include "state_functions.h"

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

void RunApp(d2d_app& app, game_state& gameState, sound_buffers& soundBuffers);
bool ProcessMessage(MSG* msg);
void FormatDiagnostics(std::list<std::wstring>& diagnostics, const game_state& gameState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");

  static const int fps = 60;

  const std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance, nCmdShow, fps);
  
  const game_state_ptr gameState = std::make_unique<game_state>(configFile.settings[L"data_path"]);
  sound_buffers_ptr soundBuffers = std::make_unique<sound_buffers>(app->directSound, configFile.settings[L"data_path"]);

  HRESULT hr = S_OK;

  hr = app->dxgi_swapChain->SetFullscreenState(TRUE, NULL);
  if( FAILED(hr) ) return 0;

  MSG msg;
  while (ProcessMessage(&msg))
  {
    RunApp(*app, *gameState, *soundBuffers);
	}

  return (int) msg.wParam;
}

void RunApp(d2d_app& app, game_state& gameState, sound_buffers& soundBuffers)
{
  if( app.terminating ) return;

  std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app.d2d_rendertarget, app.brushes, app.textFormats);

  RenderFrame(*frame, gameState);

  app.mouseCursor->xPos = app.previousControlState->renderTargetMouseX;
  app.mouseCursor->yPos = app.previousControlState->renderTargetMouseY;
  RenderMouseCursor(*frame, *app.mouseCursor);

  app.dxgi_swapChain->Present(1, 0);

  app.controlState = GetControlState(app, *app.previousControlState);

  UpdatePerformanceData(*app.perfData);

  std::list<std::wstring> diagnostics;
  FormatDiagnostics(diagnostics, gameState, *app.controlState, *app.perfData, *app.timer);
  RenderDiagnostics(*frame, diagnostics);

  const game_events_ptr events = UpdateGameState(gameState, *app.controlState, *app.timer);

  UpdateSystemTimer(*app.timer);

  UpdateSound(soundBuffers, gameState, *events);

  app.previousControlState = std::move(app.controlState);

  if( !gameState.running )
  {
    ::PostQuitMessage(0);
    app.terminating = true;
    return;
  }
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
  const play_state_ptr& playState = gameState.playState;

  static wchar_t text[64];

  float runTime = GetTotalTimeInSeconds(timer);
  float intervalTime = GetIntervalTimeInSeconds(timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnostics.push_back(text);

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnostics.push_back(text);

  if( playState )
  {
    swprintf(text, L"bullet count: %I64u", playState->bullets.size());
    diagnostics.push_back(text);
  }

  swprintf(text, L"mouse x: %i", static_cast<int>(controlState.renderTargetMouseX));
  diagnostics.push_back(text);

  swprintf(text, L"mouse y: %i", static_cast<int>(controlState.renderTargetMouseY));
  diagnostics.push_back(text);
}
