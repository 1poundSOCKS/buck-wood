#ifndef _screen_runner_
#define _screen_runner_

#include "screen_render.h"
#include "perf_data.h"
#include "diagnostics.h"
#include "main_window.h"
#include "dwrite_factory.h"
#include "screen_input_state.h"
#include "render_guard.h"
#include "framework.h"

inline bool g_closeAllScreens = false;

template <typename screen_state_type> auto KeepScreenOpen(const screen_state_type& screenState) -> bool
{
  MSG msg;

  if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  
    if( msg.message == WM_QUIT )
    {
      g_closeAllScreens = true;
    }
  }

  return !g_closeAllScreens && screenState.ContinueRunning();
}

template <typename screen_state_type> void OpenScreen()
{
  screen_state_type screenState;
  screen_input_state inputState;

  auto previousTime = performance_counter::QueryValue();
  auto currentTime = previousTime;
  
  while( KeepScreenOpen(screenState) )
  {
    inputState.windowData = framework::windowData();
    inputState.renderTargetMouseData = GetRenderTargetMouseData(inputState.windowData, framework::renderTarget().get());

    ReadKeyboardState(framework::keyboard().get(), inputState.keyboardState);

    auto timerFrequency = performance_counter::QueryFrequency();
    auto frameTime = timerFrequency / framework::fps();

    screenState.Refresh(inputState, framework::isFrameRateUnlocked() ? currentTime - previousTime : frameTime);

    previousTime = currentTime;
    currentTime = performance_counter::QueryValue();

    inputState.previousWindowData = inputState.windowData;
    inputState.previousKeyboardState = inputState.keyboardState;
    inputState.previousRenderTargetMouseData = inputState.renderTargetMouseData;
  }
}

#endif
