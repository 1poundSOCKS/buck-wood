#ifndef _screen_runner_
#define _screen_runner_

#include "screen_render.h"
#include "global_state.h"
#include "diagnostics.h"
#include "main_window.h"
#include "screen_input_state.h"

bool ProcessMessage();

inline auto GetPercentageTime(int64_t frameTicks, int64_t elapsedTime) -> float
{
  return static_cast<float>(elapsedTime) / static_cast<float>(frameTicks) * 100.0;
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

template <typename render_data_type,typename sound_data_type, typename screen_state_type>
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
  performance::frame_data frameData;
  
  while( ProcessMessage() && ContinueRunning(screenState) )
  {
    performance::UpdateFrameData(frameData);

    inputState.windowData = data.windowData;
    inputState.renderTargetMouseData = GetRenderTargetMouseData(inputState.windowData, data.renderTarget.get());

    ReadKeyboardState(data.keyboard.get(), inputState.keyboardState);

    UpdateScreen(
      data, 
      renderData, 
      soundData, 
      screenState, 
      inputState, 
      frameData,
      diagnosticsRenderData);

    inputState.previousWindowData = inputState.windowData;
    inputState.previousKeyboardState = inputState.keyboardState;
    inputState.previousRenderTargetMouseData = inputState.renderTargetMouseData;
  }
}

template <typename render_data_type, typename sound_data_type, typename screen_state_type>
void UpdateScreen(
  screen_runner_data& data,
  render_data_type& renderData,
  sound_data_type& soundData, 
  screen_state_type& screenState,
  const screen_input_state& inputState,
  const performance::frame_data& frameData,
  screen_diagnostics_render_data& diagnosticsRenderData)
{
  std::vector<std::wstring> diagnosticsData;
  diagnosticsData.reserve(50);

  auto timerFrequency = performance_counter::QueryFrequency();
  auto frameTime = timerFrequency / data.fps;
  
  auto startUpdateTime = performance_counter::QueryValue();
  UpdateScreenState(screenState, inputState);
  auto endUpdateTime = performance_counter::QueryValue();

  diagnosticsData.emplace_back(std::format(L"update state time: {:.1f}", GetPercentageTime(frameTime, endUpdateTime - startUpdateTime)));

  FormatDiagnostics(inputState, std::back_inserter(diagnosticsData));
  diagnosticsData.emplace_back(std::format(L"fps: {}", performance::GetFPS(frameData)));
  FormatDiagnostics(screenState, std::back_inserter(diagnosticsData));

  if( inputState.keyboardState.data[DIK_F12] & 0x80 && !(inputState.previousKeyboardState.data[DIK_F12] & 0x80) )
  {
    data.swapChain->SetFullscreenState(TRUE, NULL);
  }

  {
    render_guard renderGuard(data.renderTarget);

    auto startRenderTime = performance_counter::QueryValue();
    RenderFrame(data.renderTarget.get(), renderData, screenState);
    auto endRenderTime = performance_counter::QueryValue();

    diagnosticsData.emplace_back(std::format(L"render time: {:.1f}", GetPercentageTime(frameTime, endRenderTime - startRenderTime)));
    
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
}

#endif
