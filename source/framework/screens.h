#ifndef _screens_
#define _screens_

#include "d2d_app.h"
#include "global_state.h"
#include "control_state.h"
#include "diagnostics.h"

enum screen_status { screen_active, screen_closed };

template<typename screen_state_type> screen_status GetScreenStatus(const screen_state_type& screenState);

bool ProcessMessage();

template<typename screen_state_type, typename control_state_type>
void RunScreen(d2d_app& app, global_state& globalState)
{
  screen_state_type screenState(app, globalState);

  screen_status screenStatus = screen_active;

  while( ProcessMessage() && screenStatus == screen_active )
  {
    UpdateScreen<screen_state_type, control_state_type>(app, globalState, screenState);
    screenStatus = GetScreenStatus(screenState);
	}

  UpdateGlobalState(globalState, screenState);
}

template<typename screen_state_type, typename control_state_type>
void UpdateScreen(d2d_app& app, const global_state& globalState, screen_state_type& screenState)
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

  static control_state_type screenControlState;
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

#endif
