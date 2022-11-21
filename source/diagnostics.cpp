#include "pch.h"
#include "diagnostics.h"

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const perf_data& perfData, const system_timer& timer)
{
  static wchar_t text[64];

  float runTime = GetTotalTimeInSeconds(timer);
  float intervalTime = GetIntervalTimeInSeconds(timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnosticsData = text;

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnosticsData = text;
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const global_state& globalState)
{
  static wchar_t text[64];

  swprintf(text, L"levels loaded: %I64u", globalState.gameLevelDataIndex->gameLevelData.size());
  diagnosticsData = text;
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const control_state& controlState)
{
  static wchar_t text[64];

  swprintf(text, L"render target size: %.1f, %.1f", controlState.renderTargetMouseData.size.width, controlState.renderTargetMouseData.size.height);
  diagnosticsData = text;

  swprintf(text, L"mouse: %.1f, %.1f", controlState.renderTargetMouseData.x, controlState.renderTargetMouseData.y);
  diagnosticsData = text;
}
