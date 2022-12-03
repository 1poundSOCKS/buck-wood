#ifndef _screen_runner_
#define _screen_runner_

#include "screen_render.h"
#include "global_state.h"
#include "control_state.h"
#include "diagnostics.h"
#include "main_window.h"
#include "control_state_reader.h"

bool ProcessMessage();

template<
  // typename screen_state_type, 
  // typename control_state_type, 
  typename screen_render_brush_selector_type,
  typename screen_render_text_format_selector_type,
  typename sound_buffer_selector_type
>
struct screen_runner
{
  IDXGISwapChain* swapChain;
  ID2D1RenderTarget* renderTarget;
  IDirectInputDevice8* keyboard;
  const window_data& windowData;
  system_timer& systemTimer;
  perf_data& perfData;
  screen_render_brush_selector_type renderBrushSelector;
  screen_render_text_format_selector_type textFormatSelector;
  sound_buffer_selector_type soundBufferSelector;

  template <typename screen_state_type, typename control_state_reader_type>
  void Start(screen_state_type& screenState)
  {
    bool continueRunning = true;

    keyboard_state keyboardState;
    window_data previousWindowData = windowData;
    
    while( ProcessMessage() && ContinueRunning(screenState) )
    {
      keyboard_state previousKeyboardState = keyboardState;
      keyboard_state_reader keyboardStateReader { keyboard };
      keyboardStateReader.Read(keyboardState);

      control_state_reader_type controlStateReader { windowData, previousWindowData, keyboardState, previousKeyboardState };
      UpdateScreen<screen_state_type, control_state_reader_type>(screenState, controlStateReader);
      previousWindowData = windowData;
    }
  }

  template <typename screen_state_type, typename control_state_reader_type>
  void UpdateScreen(screen_state_type& screenState, const control_state_reader_type& controlStateReader)
  {
    // static input_state inputState, previousInputState;
    // previousInputState = inputState;
    // inputState.RefreshKeyboard(keyboard);
    // inputState.clientMouseData.rect = windowData.mouse.rect;
    // inputState.clientMouseData.x = windowData.mouse.x;
    // inputState.clientMouseData.y = windowData.mouse.y;
    // inputState.renderTargetMouseData.size = renderTarget->GetSize();
    // inputState.renderTargetMouseData.x = windowData.mouse.x;
    // inputState.renderTargetMouseData.y = windowData.mouse.y;
    // inputState.leftMouseButtonDown = windowData.mouse.leftButtonDown;
    // inputState.rightMouseButtonDown = windowData.mouse.rightButtonDown;

    // static control_state baseControlState;
    // baseControlState.Refresh(inputState, previousInputState);

    // static control_state_type screenControlState;
    // RefreshControlState(screenControlState, baseControlState);

    auto start = QueryPerformanceCounter();
    // UpdateScreenState(screenState, screenControlState, systemTimer);
    UpdateScreenState(screenState, controlStateReader, systemTimer);
    auto end = QueryPerformanceCounter();
    perfData.updateScreenStateTicks = end - start;

    static diagnostics_data diagnosticsData;
    diagnosticsData.clear();
    diagnosticsData.reserve(50);
    FormatDiagnostics(std::back_inserter(diagnosticsData), perfData, systemTimer);
    // FormatDiagnostics(std::back_inserter(diagnosticsData), baseControlState);
    // FormatDiagnostics(std::back_inserter(diagnosticsData), screenState, screenControlState);
    // FormatDiagnostics(std::back_inserter(diagnosticsData), screenState, controlState);

    // if( baseControlState.f12Press ) swapChain->SetFullscreenState(TRUE, NULL);

    {
      d2d_frame frame(renderTarget);
      auto start = QueryPerformanceCounter();
      RenderFrame(renderTarget, renderBrushSelector, textFormatSelector, screenState);
      auto end = QueryPerformanceCounter();
      perfData.renderFrameTicks = end - start;
      renderTarget->SetTransform(D2D1::IdentityMatrix());
      RenderText(renderTarget, renderBrushSelector[grey], textFormatSelector[diagnostics], GetDiagnosticsString(diagnosticsData.cbegin(), diagnosticsData.cend()));
    }

    swapChain->Present(1, 0);

    PlaySoundEffects(screenState, soundBufferSelector);

    UpdateTimer(systemTimer);
    UpdatePerformanceData(perfData);
  }
};

#endif
