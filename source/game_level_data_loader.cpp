#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"

game_level_data_loader::game_level_data_loader() : m_currentLevelDataIterator(global_state::firstLevelData())
{
}

auto game_level_data_loader::SetPlayerPositionUpdate(player_ship::position_update playerPositionUpdate) -> void
{
  m_playerPositionUpdate = playerPositionUpdate;
}

auto game_level_data_loader::SetPlayerShot(player_ship::event_shot playerShot) -> void
{
  m_playerShot = playerShot;
}

auto game_level_data_loader::SetPlayerDied(player_ship::event_died playerDied) -> void
{
  m_playerDied = playerDied;
}

auto game_level_data_loader::SetTargetActivated(level_target::event_activated targetActivated) -> void
{
  m_targetActivated = targetActivated;
}

auto game_level_data_loader::SetTimeout(level_container::timeout timeoutEvent) -> void
{
  m_timeoutEvent = timeoutEvent;
}

auto game_level_data_loader::LoadLevel(ID2D1RenderTarget* renderTarget) const -> level_container
{
  level_container levelContainer;
  levelContainer.GetObjectContainer().Initialize(renderTarget);
  LoadIslands(levelContainer.GetObjectContainer());
  LoadTargets(levelContainer.GetObjectContainer());
  LoadPlayer(levelContainer.GetObjectContainer());
  levelContainer.SetTimeout(GetTimeLimit(), m_timeoutEvent);
  return levelContainer;
}

auto game_level_data_loader::LoadIslands(active_object_container& levelObjectContainer) const -> void
{
  LoadIslands(**m_currentLevelDataIterator, levelObjectContainer);
}

auto game_level_data_loader::LoadTargets(active_object_container& levelObjectContainer) const -> void
{
  LoadTargets(**m_currentLevelDataIterator, levelObjectContainer);
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(active_object_container& levelObjectContainer) const -> void
{
  LoadPlayer(**m_currentLevelDataIterator, levelObjectContainer);
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

auto game_level_data_loader::LoadTargets(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void
{
  std::vector<level_target> targets;
  std::for_each(levelData.targets.cbegin(), levelData.targets.cend(), [this, &levelObjectContainer](const auto& position) -> void
  {
    levelObjectContainer.AppendActiveObject(level_target { position.x, position.y, m_targetActivated });
  });
}

[[nodiscard]] auto game_level_data_loader::LoadPlayer(const game_level_data& levelData, active_object_container& levelObjectContainer) const -> void
{
  player_ship playerShip { levelData.playerStartPosX, levelData.playerStartPosY };

  if( m_playerPositionUpdate ) playerShip.SetPositionUpdate(m_playerPositionUpdate);
  if( m_playerShot ) playerShip.SetEventShot(m_playerShot);
  if( m_playerDied ) playerShip.SetEventDied(m_playerDied);
  
  levelObjectContainer.AppendActiveObject(playerShip);
}
