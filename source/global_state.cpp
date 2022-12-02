#include "pch.h"
#include "global_state.h"

global_state::global_state(const std::wstring& dataPath/*, ID2D1RenderTarget* renderTarget, IDirectSound8* directSound*/)
: dataPath(dataPath)/*, 
  soundBuffers(directSound, dataPath)*/
{
  gameLevelDataIndex = LoadAllGameLevelData(dataPath);
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const global_state& globalState)
{
  static wchar_t text[64];

  swprintf(text, L"levels loaded: %I64u", globalState.gameLevelDataIndex->gameLevelData.size());
  diagnosticsData = text;
}
