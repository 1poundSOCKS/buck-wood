#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"

game_level_data_loader::game_level_data_loader() : m_currentLevelDataIterator(global_state::firstLevelData())
{
}

auto game_level_data_loader::LoadIslands(std::back_insert_iterator<std::vector<level_island>> islandInserter) const -> void
{
  LoadIslands(**m_currentLevelDataIterator, islandInserter);
}

auto game_level_data_loader::LoadTargets(std::back_insert_iterator<std::vector<target_state>> targetInserter) const -> void
{
  LoadTargets(**m_currentLevelDataIterator, targetInserter);
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer() const -> std::unique_ptr<player_ship>
{
  return LoadPlayer(**m_currentLevelDataIterator);
}

[[nodiscard]] auto game_level_data_loader::LoadTimer() const -> std::unique_ptr<level_timer>
{
  return LoadTimer(**m_currentLevelDataIterator);
}

auto game_level_data_loader::NextLevel() -> void
{
}

[[nodiscard]] auto game_level_data_loader::EndOfLevels() const -> bool
{
  return m_currentLevelDataIterator == global_state::endLevelData();
}

auto game_level_data_loader::LoadIslands(const game_level_data& levelData, std::back_insert_iterator<std::vector<level_island>> islandInserter) const -> void
{
  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::transform(levelObjects.cbegin(), levelObjects.cend(), islandInserter, [this](const auto& object) -> level_island
  {
    return { object };
  });
}

auto game_level_data_loader::LoadTargets(const game_level_data& levelData, std::back_insert_iterator<std::vector<target_state>> targetInserter) const -> void
{
  std::vector<target_state> targets;
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), targetInserter, [this](const auto& position) -> target_state
  {
    return { position };
  });
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(const game_level_data& levelData) const -> std::unique_ptr<player_ship>
{
  auto playerShip = std::make_unique<player_ship>();
  playerShip->SetPosition(levelData.playerStartPosX, levelData.playerStartPosY);
  return playerShip;
}

[[nodiscard]] auto game_level_data_loader::LoadTimer(const game_level_data& levelData) const -> std::unique_ptr<level_timer>
{
  return std::make_unique<level_timer>(levelData.timeLimitInSeconds);
}
