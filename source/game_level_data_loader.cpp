#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"

game_level_data_loader::game_level_data_loader() : m_currentLevelDataIterator(global_state::firstLevelData())
{
}

auto game_level_data_loader::LoadLevel(active_object_container& levelObjectContainer, passive_object_container& overlayObjectContainer, 
  player_ship::position_update playerPositionUpdate, player_ship::event_shot playerShot, 
  player_ship::event_died playerDied, std::function<void()> eventTargetActivated) const -> const level_control_data
{
  LoadIslands(levelObjectContainer);
  LoadTargets(levelObjectContainer, eventTargetActivated);

  LoadPlayer(levelObjectContainer, playerPositionUpdate, playerShot, playerDied);
  auto timerControlData = LoadTimer(overlayObjectContainer);
  auto stateControlData = LoadState(overlayObjectContainer);

  return { timerControlData, stateControlData };
}

auto game_level_data_loader::LoadIslands(active_object_container& levelObjectContainer) const -> void
{
  LoadIslands(**m_currentLevelDataIterator, levelObjectContainer);
}

auto game_level_data_loader::LoadTargets(active_object_container& levelObjectContainer, std::function<void()> eventTargetActivated) const -> void
{
  LoadTargets(**m_currentLevelDataIterator, levelObjectContainer, eventTargetActivated);
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(active_object_container& levelObjectContainer, player_ship::position_update playerPositionUpdate, player_ship::event_shot playerShot, player_ship::event_died playerDied) const -> void
{
  LoadPlayer(**m_currentLevelDataIterator, levelObjectContainer, playerPositionUpdate, playerShot, playerDied);
}

[[nodiscard]] auto game_level_data_loader::LoadTimer(passive_object_container& levelObjectContainer) const -> level_timer::control_data
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

auto game_level_data_loader::LoadIslands(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void
{
  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::for_each(levelObjects.cbegin(), levelObjects.cend(), [this, &levelObjectContainer](const auto& object) -> void
  {
    levelObjectContainer.AppendActiveObject(level_island { object });
  });
}

auto game_level_data_loader::LoadTargets(const game_level_data& levelData, active_object_container& levelObjectContainer, 
  std::function<void()> eventTargetActivated) const -> void
{
  std::vector<level_target> targets;
  std::for_each(levelData.targets.cbegin(), levelData.targets.cend(), [&levelObjectContainer,&eventTargetActivated](const auto& position) -> void
  {
    levelObjectContainer.AppendActiveObject(level_target { position.x, position.y, eventTargetActivated });
  });
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(const game_level_data& levelData, active_object_container& levelObjectContainer, 
  player_ship::position_update playerPositionUpdate, player_ship::event_shot playerShot, player_ship::event_died playerDied) const -> void
{
  player_ship playerShip { levelData.playerStartPosX, levelData.playerStartPosY };
  playerShip.SetPositionUpdate(playerPositionUpdate);
  playerShip.SetEventShot(playerShot);
  playerShip.SetEventDied(playerDied);
  levelObjectContainer.AppendActiveObject(playerShip);
}

[[nodiscard]] auto game_level_data_loader::LoadTimer(const game_level_data& levelData, passive_object_container& levelObjectContainer) const -> level_timer::control_data
{
  level_timer levelTimer { levelData.timeLimitInSeconds * performance_counter::QueryFrequency() };
  levelObjectContainer.AppendOverlayObject(levelTimer);
  return levelTimer.GetControlData();
}

[[nodiscard]] auto game_level_data_loader::LoadState(passive_object_container& levelObjectContainer) const -> level_state::control_data
{
  level_state levelState;
  levelObjectContainer.AppendOverlayObject(levelState);
  return levelState.GetControlData();
}
