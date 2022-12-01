#ifndef _screens_
#define _screens_

#include "screen_render.h"
#include "global_state.h"
#include "control_state.h"
#include "diagnostics.h"
#include "main_window.h"

enum screen_status { screen_active, screen_closed };

template<typename screen_state_type> screen_status GetScreenStatus(const screen_state_type& screenState);
bool ProcessMessage();

template<typename global_state_type, typename screen_state_type, typename control_state_type>
void RunScreen(IDXGISwapChain* swapChain,
               ID2D1RenderTarget* renderTarget,
               IDirectInputDevice8* keyboard, 
               const window_data& windowData, 
               system_timer& systemTimer, 
               perf_data& perfData, global_state_type& globalState)
{
  screen_state_type screenState(systemTimer, globalState);

  screen_render_brushes renderBrushes;
  CreateScreenRenderBrushes(renderTarget, renderBrushes);
  screen_render_brush_selector renderBrushSelector { renderBrushes };

  screen_render_text_formats textFormats;
  CreateScreenRenderTextFormats(textFormats);
  screen_render_text_format_selector textFormatSelector { textFormats};

  screen_status screenStatus = screen_active;

  while( ProcessMessage() && screenStatus == screen_active )
  {
    UpdateScreen<global_state_type, screen_state_type, control_state_type>
      (swapChain, renderTarget, renderBrushSelector, textFormatSelector, keyboard, windowData, systemTimer, perfData, globalState, screenState);

    screenStatus = GetScreenStatus(screenState);
	}

  UpdateGlobalState(globalState, screenState);
}

template<typename global_state_type, typename screen_state_type, typename control_state_type>
void UpdateScreen(IDXGISwapChain* swapChain,
                  ID2D1RenderTarget* renderTarget,
                  screen_render_brush_selector renderBrushSelector,
                  screen_render_text_format_selector textFormatSelector,
                  IDirectInputDevice8* keyboard, 
                  const window_data& windowData, 
                  system_timer& systemTimer, 
                  perf_data& perfData, 
                  const global_state_type& globalState, 
                  screen_state_type& screenState)
{
  static input_state inputState, previousInputState;
  previousInputState = inputState;
  inputState.RefreshKeyboard(keyboard);
  inputState.clientMouseData.rect = windowData.mouse.rect;
  inputState.clientMouseData.x = windowData.mouse.x;
  inputState.clientMouseData.y = windowData.mouse.y;
  inputState.renderTargetMouseData.size = renderTarget->GetSize();
  inputState.renderTargetMouseData.x = windowData.mouse.x;
  inputState.renderTargetMouseData.y = windowData.mouse.y;
  inputState.leftMouseButtonDown = windowData.mouse.leftButtonDown;
  inputState.rightMouseButtonDown = windowData.mouse.rightButtonDown;

  static control_state baseControlState;
  baseControlState.Refresh(inputState, previousInputState);

  static control_state_type screenControlState;
  RefreshControlState(screenControlState, baseControlState);

  auto start = QueryPerformanceCounter();
  UpdateScreenState(screenState, screenControlState, systemTimer);
  auto end = QueryPerformanceCounter();
  perfData.updateScreenStateTicks = end - start;

  static diagnostics_data diagnosticsData;
  diagnosticsData.clear();
  diagnosticsData.reserve(50);
  FormatDiagnostics(std::back_inserter(diagnosticsData), perfData, systemTimer);
  FormatDiagnostics(std::back_inserter(diagnosticsData), globalState);
  FormatDiagnostics(std::back_inserter(diagnosticsData), baseControlState);
  FormatDiagnostics(std::back_inserter(diagnosticsData), screenState, screenControlState);

  if( baseControlState.f12Press ) swapChain->SetFullscreenState(TRUE, NULL);

  {
    d2d_frame frame(renderTarget);
    auto start = QueryPerformanceCounter();
    RenderFrame(renderTarget, renderBrushSelector, textFormatSelector, screenState);
    auto end = QueryPerformanceCounter();
    perfData.renderFrameTicks = end - start;
    renderTarget->SetTransform(D2D1::IdentityMatrix());
    RenderText(renderTarget, renderBrushSelector[grey], textFormatSelector[diagnostics], GetDiagnosticsString(diagnosticsData.cbegin(), diagnosticsData.cend()));
    RenderMouseCursor(renderTarget, renderBrushSelector[white], baseControlState.renderTargetMouseData.x, baseControlState.renderTargetMouseData.y);
  }

  swapChain->Present(1, 0);

  PlaySoundEffects(screenState);

  UpdateTimer(systemTimer);
  UpdatePerformanceData(perfData);
}

#endif
