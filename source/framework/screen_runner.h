#ifndef _screen_runner_
#define _screen_runner_

#include "screen_render.h"
#include "global_state.h"
#include "diagnostics.h"
#include "main_window.h"
#include "control_state_reader.h"

bool ProcessMessage();

template<
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

  template <typename screen_state_type>
  void Start(screen_state_type& screenState)
  {
    bool continueRunning = true;

    screen_input_state inputState;
    
    while( ProcessMessage() && ContinueRunning(screenState) )
    {
      inputState.windowData = windowData;

      keyboard_state_reader keyboardStateReader { keyboard };
      keyboardStateReader.Read(inputState.keyboardState);

      UpdateScreen<screen_state_type>(screenState, inputState);

      inputState.previousWindowData = inputState.windowData;
      inputState.previousKeyboardState = inputState.keyboardState;
    }
  }

  template <typename screen_state_type>
  void UpdateScreen(screen_state_type& screenState, const screen_input_state& inputState)
  {
    auto start = QueryPerformanceCounter();
    UpdateScreenState(screenState, inputState, systemTimer);
    auto end = QueryPerformanceCounter();
    perfData.updateScreenStateTicks = end - start;

    static diagnostics_data diagnosticsData;
    diagnosticsData.clear();
    diagnosticsData.reserve(50);
    FormatDiagnostics(std::back_inserter(diagnosticsData), perfData, systemTimer);
    
    if( inputState.keyboardState.data[DIK_F12] & 0x80 && !(inputState.previousKeyboardState.data[DIK_F12] & 0x80) )
    {
      swapChain->SetFullscreenState(TRUE, NULL);
    }

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
