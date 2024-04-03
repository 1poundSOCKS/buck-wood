#include "pch.h"
#include "game_state.h"

[[nodiscard]] auto game_state::Saved() noexcept -> bool
{
  return m_levelIndex > 0;
}
