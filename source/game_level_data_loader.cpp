#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"
#include "level_object_container.h"

game_level_data_loader::game_level_data_loader() : m_currentLevelDataIterator(global_state::firstLevelData())
{
}

auto game_level_data_loader::LoadIslands(level_object_container& levelObjectContainer) const -> void
{
  LoadIslands(**m_currentLevelDataIterator, levelObjectContainer);
}

auto game_level_data_loader::LoadTargets(level_object_container& levelObjectContainer) const -> void
{
  LoadTargets(**m_currentLevelDataIterator, levelObjectContainer);
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(level_object_container& levelObjectContainer) const -> player_ship::control_data
{
  return LoadPlayer(**m_currentLevelDataIterator, levelObjectContainer);
}

[[nodiscard]] auto game_level_data_loader::LoadTimer(level_object_container& levelObjectContainer) const -> level_timer::control_data
{
  return LoadTimer(**m_currentLevelDataIterator, levelObjectContainer);
}

[[nodiscard]] auto game_level_data_loader::GetTimeLimit() const -> int64_t
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

auto game_level_data_loader::LoadIslands(const game_level_data& levelData, level_object_container& levelObjectContainer) const -> void
{
  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::for_each(levelObjects.cbegin(), levelObjects.cend(), [this, &levelObjectContainer](const auto& object) -> void
  {
    levelObjectContainer.AppendActiveObject(level_island { object });
  });
}

auto game_level_data_loader::LoadTargets(const game_level_data& levelData, level_object_container& levelObjectContainer) const -> void
{
  std::vector<target_state> targets;
  std::for_each(levelData.targets.cbegin(), levelData.targets.cend(), [&levelObjectContainer](const auto& position) -> void
  {
    levelObjectContainer.AppendActiveObject(target_state { position.x, position.y });
  });
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(const game_level_data& levelData, level_object_container& levelObjectContainer) const -> player_ship::control_data
{
  auto controlData = std::make_shared<player_ship::control>();
  levelObjectContainer.AppendActiveObject(player_ship { controlData });
  return controlData;
}

[[nodiscard]] auto game_level_data_loader::LoadTimer(const game_level_data& levelData, level_object_container& levelObjectContainer) const -> level_timer::control_data
{
  auto controlData = std::make_shared<level_timer::control>();
  levelObjectContainer.AppendOverlayObject(level_timer { controlData });
  return controlData;
}

[[nodiscard]] auto game_level_data_loader::LoadState(level_object_container& levelObjectContainer) const -> level_state::control_data
{
  auto controlData = std::make_shared<level_state::control>();
  levelObjectContainer.AppendOverlayObject(level_state { controlData });
  return controlData;
}
