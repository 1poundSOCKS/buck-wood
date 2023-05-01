#include "pch.h"
#include "global_state.h"

global_state* global_state::m_globalState = nullptr;

global_state::global_state(const std::wstring& dataPath) : dataPath(dataPath)
{
  // gameLevelDataIndex = LoadAllGameLevelData(dataPath);
}

auto global_state::load(const std::wstring& dataPath) -> void
{
  m_globalState = new global_state(dataPath);
}

auto global_state::save() -> void
{
  // SaveAllGameLevelData(*m_globalState->gameLevelDataIndex);
}

// [[nodiscard]] auto global_state::firstLevelData() -> game_level_data_index::const_iterator
// {
//   return m_globalState->gameLevelDataIndex->gameLevelData.cbegin();
// }

// [[nodiscard]] auto global_state::endLevelData() -> game_level_data_index::const_iterator
// {
//   return m_globalState->gameLevelDataIndex->gameLevelData.cend();
// }
