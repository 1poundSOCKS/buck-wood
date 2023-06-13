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

inline auto GetPercentageTime(int64_t frameTicks, int64_t elapsedTime) -> float
{
  return static_cast<float>(elapsedTime) / static_cast<float>(frameTicks) * 100.0f;
}

struct screen_runner_data
{
  winrt::com_ptr<IDXGISwapChain> swapChain;
  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  winrt::com_ptr<IDWriteFactory> dwriteFactory;
  winrt::com_ptr<IDirectInputDevice8> keyboard;
  const window_data& windowData;
  int fps;
};

struct screen_diagnostics_render_data
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<IDWriteTextFormat> textFormat;
};

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
  screen_runner_data data
  {
    framework::swapChain(),
    framework::renderTarget(), 
    dwrite_factory::get(),
    framework::keyboard(), 
    framework::windowData(),
    framework::fps()
  };

  screen_diagnostics_render_data diagnosticsRenderData
  {
    CreateScreenRenderBrush(data.renderTarget.get(), D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f)),    
    CreateScreenRenderTextFormat(data.dwriteFactory.get(), L"Verdana", DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20)
  };

  screen_state_type screenState;
  screen_input_state inputState;
  performance::frame_data frameData;

  auto previousTime = performance_counter::QueryValue();
  auto currentTime = previousTime;
  
  while( KeepScreenOpen(screenState) )
  {
    performance::UpdateFrameData(frameData);

    inputState.windowData = data.windowData;
    inputState.renderTargetMouseData = GetRenderTargetMouseData(inputState.windowData, data.renderTarget.get());

    ReadKeyboardState(data.keyboard.get(), inputState.keyboardState);

    UpdateScreen(data, screenState, inputState, currentTime - previousTime, frameData, diagnosticsRenderData, framework::isFrameRateUnlocked());

    previousTime = currentTime;
    currentTime = performance_counter::QueryValue();

    inputState.previousWindowData = inputState.windowData;
    inputState.previousKeyboardState = inputState.keyboardState;
    inputState.previousRenderTargetMouseData = inputState.renderTargetMouseData;
  }
}

void UpdateScreen(screen_runner_data& data, auto& screenState, const screen_input_state& inputState, int64_t frameInterval, 
  const performance::frame_data& frameData, screen_diagnostics_render_data& diagnosticsRenderData, bool unlockFrameRate)
{
  std::vector<std::wstring> diagnosticsData;
  diagnosticsData.reserve(50);

  auto timerFrequency = performance_counter::QueryFrequency();
  auto frameTime = timerFrequency / data.fps;
  
  auto startUpdateTime = performance_counter::QueryValue();
  screenState.Update(inputState, unlockFrameRate ? frameInterval : frameTime);
  auto endUpdateTime = performance_counter::QueryValue();

  diagnosticsData.emplace_back(std::format(L"update state time: {:.1f}", GetPercentageTime(frameTime, endUpdateTime - startUpdateTime)));

  FormatDiagnostics(inputState, std::back_inserter(diagnosticsData));
  diagnosticsData.emplace_back(std::format(L"fps: {}", performance::GetFPS(frameData)));
  screenState.FormatDiagnostics(std::back_inserter(diagnosticsData));

  if( inputState.keyboardState.data[DIK_F12] & 0x80 && !(inputState.previousKeyboardState.data[DIK_F12] & 0x80) )
  {
    BOOL fullScreen = FALSE;
    data.swapChain->GetFullscreenState(&fullScreen, nullptr);
    data.swapChain->SetFullscreenState(fullScreen ? FALSE : TRUE, nullptr);
  }

  {
    render_guard renderGuard(data.renderTarget);

    auto startRenderTime = performance_counter::QueryValue();
    screenState.Render();
    auto endRenderTime = performance_counter::QueryValue();

    diagnosticsData.emplace_back(std::format(L"render time: {:.1f}", GetPercentageTime(frameTime, endRenderTime - startRenderTime)));
    
    data.renderTarget->SetTransform(D2D1::IdentityMatrix());

    RenderText(data.renderTarget.get(), diagnosticsRenderData.brush.get(), diagnosticsRenderData.textFormat.get(), 
      GetDiagnosticsString(diagnosticsData.cbegin(), diagnosticsData.cend()));
  }

  data.swapChain->Present(unlockFrameRate ? 0 : 1, 0);

  screenState.PostPresent();
}

#endif
