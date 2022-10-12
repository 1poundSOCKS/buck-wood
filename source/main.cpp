#define UNICODE

#include <iostream>
#include <tchar.h>
#include <math.h>

#include "math.h"
#include "d2d_app.h"
#include "render.h"
#include "control_state.h"
#include "game_level.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")
#pragma comment(lib,"jsoncpp.lib")
#pragma comment(lib, "RuntimeObject.lib")

bool ProcessMessage(MSG* msg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
  const int fps = 60;
  const float frameTime = 1.0f / static_cast<float>(fps);
  
  std::unique_ptr<d2d_app> app = std::make_unique<d2d_app>(hInstance, nCmdShow);

  std::unique_ptr<game_state> gameState = CreateInitialGameState();

  MSG msg;

  LARGE_INTEGER perfFrequency;
  QueryPerformanceFrequency(&perfFrequency);

  LARGE_INTEGER initialTicks,ticks,previousTicks;
  ticks.QuadPart = 0;
  
  QueryPerformanceCounter(&initialTicks);
	
  std::unique_ptr<control_state> previousControlState = std::make_unique<control_state>();

  std::unique_ptr<mouse_cursor> mouseCursor = std::make_unique<mouse_cursor>();

  while (ProcessMessage(&msg))
  {
    if( app->terminating ) continue;

    previousTicks = ticks;
    QueryPerformanceCounter(&ticks);

    std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(app->d2d_rendertarget);
    
    D2D1::Matrix3x2F viewTransform = CreateViewTransform(frame->renderTarget, gameState->currentLevel->width, 400.0f - gameState->player->yPos);

    RenderFrame(*frame, *gameState, viewTransform);

    std::unique_ptr<control_state> controlState = GetControlState(*app, *previousControlState);

    D2D1_SIZE_F renderTargetSize = frame->renderTarget->GetSize();
    controlState->renderTargetMouseX = controlState->mouseX * renderTargetSize.width;
    controlState->renderTargetMouseY = controlState->mouseY * renderTargetSize.height;

    if( viewTransform.Invert() )
    {
      D2D1_POINT_2F inPoint;
      inPoint.x = controlState->renderTargetMouseX;
      inPoint.y = controlState->renderTargetMouseY;
      D2D1_POINT_2F outPoint = viewTransform.TransformPoint(inPoint);
      controlState->gameMouseX = outPoint.x;
      controlState->gameMouseY = outPoint.y;
      mouseCursor->xPos = controlState->renderTargetMouseX;
      mouseCursor->yPos = controlState->renderTargetMouseY;
    }

    const std::unique_ptr<perf_data> perfData = std::make_unique<perf_data>(perfFrequency,initialTicks,ticks,previousTicks);

    wchar_t text[32];
    wsprintf(text, L"mouse x: %i", static_cast<int>(controlState->renderTargetMouseX));
    perfData->additionalInfo.push_back(text);

    wsprintf(text, L"mouse y: %i", static_cast<int>(controlState->renderTargetMouseY));
    perfData->additionalInfo.push_back(text);

    RenderDiagnostics(*frame, *gameState, *perfData);

    RenderMouseCursor(*frame, *mouseCursor);

    app->dxgi_swapChain->Present(1, 0);

    UpdateGameState(*gameState, *controlState, frameTime);

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
