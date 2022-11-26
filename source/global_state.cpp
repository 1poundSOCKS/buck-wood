#include "pch.h"
#include "global_state.h"

global_state::global_state(const d2d_app& app, const std::wstring& dataPath)
: dataPath(dataPath), 
  renderBrushes(app.d2d_rendertarget),
  soundBuffers(app.directSound, dataPath)
{
  gameLevelDataIndex = LoadAllGameLevelData(dataPath);
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const global_state& globalState)
{
  static wchar_t text[64];

  swprintf(text, L"levels loaded: %I64u", globalState.gameLevelDataIndex->gameLevelData.size());
  diagnosticsData = text;
}
