#include "pch.h"
#include "save_data.h"

save_data::save_data(std::wstring folder) noexcept : m_folder { folder }
{
}

auto save_data::Read(game_state *gameState) noexcept -> void
{
  std::filesystem::path fullFilename = m_folder;
  fullFilename /= L"game_state.dat";

  std::ifstream fileReader(fullFilename.c_str(), std::ifstream::binary);

  struct game_state_data
  {
    uint32_t levelIndex { 0 };
    uint32_t score { 0 };
    uint32_t powerups { 0 };
  };

  if( !fileReader.fail() )
  {
    game_state_data savedData;
    fileReader.read(reinterpret_cast<char*>(&savedData), sizeof(savedData));
    gameState->set_level_index(savedData.levelIndex);
    gameState->set_score(savedData.score);
    gameState->set_power_up_count(savedData.powerups);
  }
}

auto save_data::Write(const game_state* gameState) noexcept -> void
{
}
