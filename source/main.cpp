#define UNICODE

#include <iostream>
#include <tchar.h>
#include <math.h>
#include <filesystem>

#include "math.h"
#include "d2d_app.h"
#include "render.h"
#include "control_state.h"
#include "game_level.h"
#include "system_timer.h"
#include "game_data.h"
#include "sound_buffers.h"

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

bool ProcessMessage(MSG* msg);
void FormatDiagnostics(std::list<std::wstring>& diagnostics, const game_state& gameState, const control_state& controlState, const perf_data& perfData);
D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float levelWidth, float playerPosY);
void UpdateSound(const sound_buffers& soundBuffers, const game_state& gameState);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
  wchar_t currentDirectory[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, currentDirectory);

  config_file configFile(L"config.txt");

  const std::unique_ptr<perf_data> perfData = std::make_unique<perf_data>();
  const std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance, nCmdShow);
  const std::unique_ptr<game_state> gameState = CreateInitialGameState();
  const std::unique_ptr<mouse_cursor> mouseCursor = std::make_unique<mouse_cursor>();
  std::unique_ptr<control_state> previousControlState = std::make_unique<control_state>();

  sound_buffers_ptr soundBuffers = std::make_unique<sound_buffers>(app->directSound, configFile.settings[L"data_path"]);

  HRESULT hr = S_OK;

  hr = app->dxgi_swapChain->SetFullscreenState(TRUE, NULL);
  if( FAILED(hr) ) return 0;

  MSG msg;
  while (ProcessMessage(&msg))
  {
    if( app->terminating ) continue;

    std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app->d2d_rendertarget, app->brushes, app->textFormats);

    D2D1::Matrix3x2F viewTransform = CreateViewTransform(frame->renderTarget, gameState->currentLevel->width, gameState->player->yPos);
    RenderFrame(*frame, *gameState, viewTransform);

    std::unique_ptr<control_state> controlState = GetControlState(*app, *previousControlState);

    if( viewTransform.Invert() )
    {
      D2D1_POINT_2F inPoint;
      inPoint.x = controlState->renderTargetMouseX;
      inPoint.y = controlState->renderTargetMouseY;
      D2D1_POINT_2F outPoint = viewTransform.TransformPoint(inPoint);
      controlState->gameMouseX = outPoint.x;
      controlState->gameMouseY = outPoint.y;
    }

    UpdatePerformanceData(*perfData);

    std::list<std::wstring> diagnostics;
    FormatDiagnostics(diagnostics, *gameState, *controlState, *perfData);
    RenderDiagnostics(*frame, diagnostics);

    mouseCursor->xPos = controlState->renderTargetMouseX;
    mouseCursor->yPos = controlState->renderTargetMouseY;
    RenderMouseCursor(*frame, *mouseCursor);

    UpdateGameState(*gameState, *controlState);

    app->dxgi_swapChain->Present(1, 0);

    UpdateSound(*soundBuffers, *gameState);

    previousControlState = std::move(controlState);

    if( !gameState->running )
    {
      ::PostQuitMessage(0);
      app->terminating = true;
      continue;
    }
	}

  return (int) msg.wParam;
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

void FormatDiagnostics(std::list<std::wstring>& diagnostics, const game_state& gameState, const control_state& controlState, const perf_data& perfData)
{
  static wchar_t text[64];

  float runTime = GetTotalTimeInSeconds(*gameState.timer);
  float intervalTime = GetIntervalTimeInSeconds(*gameState.timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnostics.push_back(text);

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnostics.push_back(text);

  swprintf(text, L"bullet count: %I64u", gameState.bullets.size());
  diagnostics.push_back(text);

  swprintf(text, L"mouse x: %i", static_cast<int>(controlState.renderTargetMouseX));
  diagnostics.push_back(text);

  swprintf(text, L"mouse y: %i", static_cast<int>(controlState.renderTargetMouseY));
  diagnostics.push_back(text);
}

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float levelWidth, float playerPosY)
{
  D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

  float scale = renderTargetSize.width / levelWidth;
  float shiftY = renderTargetSize.height / 2 * scale;
  
  D2D1::Matrix3x2F matrixShift = D2D1::Matrix3x2F::Translation(0, shiftY - playerPosY);
  
  D2D1_SIZE_F scaleSize;
  scaleSize.width = scale;
  scaleSize.height = scale;
  D2D1::Matrix3x2F matrixScale = D2D1::Matrix3x2F::Scale(scaleSize);

  return matrixScale * matrixShift;
}

void UpdateSound(const sound_buffers& soundBuffers, const game_state& gameState)
{
  DWORD bufferStatus = 0;
  if( SUCCEEDED(soundBuffers.thrust->buffer->GetStatus(&bufferStatus)) )
  {
    if( bufferStatus & DSBSTATUS_PLAYING )
    {
      if( gameState.screen != game_state::main || 
          gameState.playerState != game_state::player_alive ||
          !gameState.player->thrusterOn )
          {
            soundBuffers.thrust->buffer->Stop();
          }
    }
    else
    {
      if( gameState.player->thrusterOn )
      {
        soundBuffers.thrust->buffer->SetCurrentPosition(0);
        soundBuffers.thrust->buffer->Play(0, 0, DSBPLAY_LOOPING);
      }
    }
  }

  if( SUCCEEDED(soundBuffers.menuTheme->buffer->GetStatus(&bufferStatus)) )
  if( bufferStatus & DSBSTATUS_PLAYING )
  {
    if( gameState.screen != game_state::title ) soundBuffers.menuTheme->buffer->Stop();
  }
  else
  {
    if( gameState.screen == game_state::title )
    {
      soundBuffers.menuTheme->buffer->SetCurrentPosition(0);
      soundBuffers.menuTheme->buffer->Play(0, 0, DSBPLAY_LOOPING);
    }
  }

  if( gameState.events->playerShot )
  {
    soundBuffers.shoot->buffer->SetCurrentPosition(0);
    soundBuffers.shoot->buffer->Play(0, 0, 0);
  }

  if( gameState.events->targetShot )
  {
    soundBuffers.targetActivated->buffer->SetCurrentPosition(0);
    soundBuffers.targetActivated->buffer->Play(0, 0, 0);
  }
}
