#include "diagnostics.h"

void FormatDiagnostics(diagnostics_data& diagnosticsData, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  static wchar_t text[64];

  float runTime = GetTotalTimeInSeconds(timer);
  float intervalTime = GetIntervalTimeInSeconds(timer);

  swprintf(text, L"run time: %.1f", runTime);
  diagnosticsData.push_back(text);

  swprintf(text, L"fps: %i", perfData.fpsAverage);
  diagnosticsData.push_back(text);

  swprintf(text, L"mouse x: %i", static_cast<int>(controlState.renderTargetMouseX));
  diagnosticsData.push_back(text);

  swprintf(text, L"mouse y: %i", static_cast<int>(controlState.renderTargetMouseY));
  diagnosticsData.push_back(text);
}
