#ifndef _screen_runner_
#define _screen_runner_

#include "screen_render.h"
#include "global_state.h"
#include "diagnostics.h"
#include "main_window.h"
#include "screen_input_state.h"

bool ProcessMessage();

struct screen_runner_data
{
  winrt::com_ptr<IDXGISwapChain> swapChain;
  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  winrt::com_ptr<IDWriteFactory> dwriteFactory;
  winrt::com_ptr<IDirectInputDevice8> keyboard;
  const window_data& windowData;
  system_timer& systemTimer;
  perf_data& perfData;
};

template
<
  typename screen_render_brush_selector_type,
  typename screen_render_text_format_selector_type,
  typename sound_buffer_selector_type
>
struct screen_runner_bespoke_data
{
  screen_render_brush_selector_type renderBrushSelector;
  screen_render_text_format_selector_type textFormatSelector;
  sound_buffer_selector_type soundBufferSelector;
};

struct screen_diagnostics_render_data
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  winrt::com_ptr<IDWriteTextFormat> textFormat;
};

template
<
  typename render_data_type,
  typename sound_data_type,
  typename screen_state_type
>
void Start(
  screen_runner_data data,
  render_data_type renderData,
  sound_data_type soundData,
  screen_state_type& screenState)
{
  screen_diagnostics_render_data diagnosticsRenderData
  {
    CreateScreenRenderBrush(data.renderTarget.get(), D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f)),
    CreateScreenRenderTextFormat(
        data.dwriteFactory.get(), 
        L"Verdana", 
        DWRITE_FONT_WEIGHT_LIGHT, 
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20)
  };

  bool continueRunning = true;

  screen_input_state inputState;
  
  while( ProcessMessage() && ContinueRunning(screenState) )
  {
    inputState.windowData = data.windowData;
    inputState.renderTargetMouseData = GetRenderTargetMouseData(inputState.windowData, data.renderTarget.get());

    ReadKeyboardState(data.keyboard.get(), inputState.keyboardState);

    UpdateScreen(data, renderData, soundData, screenState, inputState, diagnosticsRenderData);

    inputState.previousWindowData = inputState.windowData;
    inputState.previousKeyboardState = inputState.keyboardState;
    inputState.previousRenderTargetMouseData = inputState.renderTargetMouseData;
  }
}

template
<
  typename render_data_type,
  typename sound_data_type,
  typename screen_state_type
>
void UpdateScreen(
  screen_runner_data& data,
  render_data_type& renderData,
  sound_data_type& soundData, 
  screen_state_type& screenState,
  const screen_input_state& inputState,
  screen_diagnostics_render_data& diagnosticsRenderData)
{
  auto start = QueryPerformanceCounter();
  UpdateScreenState(screenState, inputState);
  auto end = QueryPerformanceCounter();
  data.perfData.updateScreenStateTicks = end - start;

  static diagnostics_data diagnosticsData;
  diagnosticsData.clear();
  diagnosticsData.reserve(50);
  FormatDiagnostics(std::back_inserter(diagnosticsData), data.perfData, data.systemTimer);
  FormatDiagnostics(std::back_inserter(diagnosticsData), inputState);

  if( inputState.keyboardState.data[DIK_F12] & 0x80 && !(inputState.previousKeyboardState.data[DIK_F12] & 0x80) )
  {
    data.swapChain->SetFullscreenState(TRUE, NULL);
  }

  {
    d2d_frame frame(data.renderTarget.get());
    auto start = QueryPerformanceCounter();
    RenderFrame(data.renderTarget.get(), renderData, screenState);
    auto end = QueryPerformanceCounter();
    data.perfData.renderFrameTicks = end - start;
    data.renderTarget->SetTransform(D2D1::IdentityMatrix());
    
    RenderText(
      data.renderTarget.get(), 
      diagnosticsRenderData.brush.get(), 
      diagnosticsRenderData.textFormat.get(), 
      GetDiagnosticsString(diagnosticsData.cbegin(), diagnosticsData.cend())
    );
  }

  data.swapChain->Present(1, 0);

  PlaySoundEffects(screenState, soundData);

  UpdateTimer(data.systemTimer);
  UpdatePerformanceData(data.perfData);
}

#endif
