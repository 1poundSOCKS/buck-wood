#include "diagnostics.h"

void FormatDiagnostics(diagnostics_data& diagnosticsData, const global_state& globalState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  static wchar_t text[64];

  swprintf(text, L"levels loaded: %I64u", globalState.gameLevelDataIndex->gameLevelData.size());
  diagnosticsData.push_back(text);

  float runTime = GetTotalTimeInSeconds(timer);
  float intervalTime = GetIntervalTimeInSeconds(timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnosticsData.push_back(text);

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnosticsData.push_back(text);

  swprintf(text, L"render target size: %.1f, %.1f", controlState.renderTargetMouseData.size.width, controlState.renderTargetMouseData.size.height);
  diagnosticsData.push_back(text);

  swprintf(text, L"mouse: %.1f, %.1f", controlState.renderTargetMouseData.x, controlState.renderTargetMouseData.y);
  diagnosticsData.push_back(text);
}
