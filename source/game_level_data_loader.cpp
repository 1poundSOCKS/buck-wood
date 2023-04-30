#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"

game_level_data_loader::game_level_data_loader() : m_currentLevelDataIterator(global_state::firstLevelData())
{
}

auto game_level_data_loader::LoadLevel(ID2D1RenderTarget* renderTarget) const -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();
  levelContainer->GetObjectContainer().Initialize(renderTarget);
  LoadIslands(*levelContainer);
  LoadTargets(*levelContainer);
  LoadPlayer(*levelContainer);
  levelContainer->SetTimeout(GetTimeLimit());
  levelContainer->GetObjectContainer().Initialize(renderTarget);
  return levelContainer;
}

auto game_level_data_loader::LoadIslands(level_container& levelContainer) const -> void
{
  LoadIslands(**m_currentLevelDataIterator, levelContainer);
}

auto game_level_data_loader::LoadTargets(level_container& levelContainer) const -> void
{
  LoadTargets(**m_currentLevelDataIterator, levelContainer);
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(level_container& levelContainer) const -> void
{
  LoadPlayer(**m_currentLevelDataIterator, levelContainer);
}

[[nodiscard]] auto game_level_data_loader::GetTimeLimit() const -> int
{
  return (*m_currentLevelDataIterator)->timeLimitInSeconds;
}

auto game_level_data_loader::NextLevel() -> void
{
}

[[nodiscard]] auto game_level_data_loader::EndOfLevels() const -> bool
{
  return m_currentLevelDataIterator == global_state::endLevelData();
}

auto game_level_data_loader::LoadIslands(const game_level_data& levelData, level_container& levelContainer) const -> void
{
  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::for_each(levelObjects.cbegin(), levelObjects.cend(), [this, &levelContainer](const auto& object) -> void
  {
    levelContainer.GetObjectContainer().AppendActiveObject(level_island { object });
  });
}

auto game_level_data_loader::LoadTargets(const game_level_data& levelData, level_container& levelContainer) const -> void
{
  std::vector<level_target> targets;
  std::for_each(levelData.targets.cbegin(), levelData.targets.cend(), [this, &levelContainer](const auto& position) -> void
  {
    levelContainer.AddTarget(level_target { position.x, position.y });
  });
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(const game_level_data& levelData, level_container& levelContainer) const -> void
{
  levelContainer.AddPlayer(player_ship { levelData.playerStartPosX, levelData.playerStartPosY });
}
