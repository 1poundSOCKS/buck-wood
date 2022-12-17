#include "pch.h"
#include "global_state.h"

global_state::global_state(const std::wstring& dataPath)
: dataPath(dataPath)
{
  gameLevelDataIndex = LoadAllGameLevelData(dataPath);
}
