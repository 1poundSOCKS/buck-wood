#include "pch.h"
#include "diagnostics.h"

extern const int fps;

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const perf_data& perfData, const system_timer& timer)
{
  static wchar_t text[64];

  float runTime = GetTotalTimeInSeconds(timer);
  float intervalTime = GetIntervalTimeInSeconds(timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnosticsData = text;

  swprintf(text, L"frame ticks: %I64u", timer.ticksPerSecond / fps);
  diagnosticsData = text;

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnosticsData = text;

  swprintf(text, L"update screen state: %I64u", perfData.updateScreenStateTicks);
  diagnosticsData = text;

  swprintf(text, L"render frame: %I64u", perfData.renderFrameTicks);
  diagnosticsData = text;
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const screen_input_state& inputState)
{
  static wchar_t text[64];
  swprintf(text, L"mouse: %i,%i", inputState.windowData.mouse.x, inputState.windowData.mouse.y);
  diagnosticsData = text;
  swprintf(text, L"client rect: %i,%i", inputState.windowData.clientRect.right, inputState.windowData.clientRect.bottom);
  diagnosticsData = text;
}

std::wstring GetDiagnosticsString(diagnostics_data::const_iterator begin, diagnostics_data::const_iterator end)
{
  std::wstring text;
  for( auto i = begin; i != end; i++ ) { text += *i + L'\n'; }
  return text;
}
