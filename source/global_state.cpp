#include "pch.h"
#include "global_state.h"

global_state* global_state::m_globalState = nullptr;

global_state::global_state(const std::wstring& dataPath) : dataPath(dataPath)
{
}

auto global_state::load(const std::wstring& dataPath) -> void
{
  m_globalState = new global_state(dataPath);
}

auto global_state::save() -> void
{
}
