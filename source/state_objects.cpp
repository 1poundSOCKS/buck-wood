#include "state_objects.h"
#include "data_files.h"

play_state::play_state(const system_timer& timer, const game_data_ptr& gameDataPtr) : gameData(gameDataPtr)
{
  state = play_state::state_playing;
  
  totalTicks = timer.totalTicks;
  ticksPerSecond = timer.ticksPerSecond;
  levelTimerStart = timer.totalTicks;
  lastShotTicks = timer.totalTicks;

  currentLevelDataIterator = gameData->begin();
  const game_level_data_ptr& levelData = *currentLevelDataIterator;
  currentLevel = std::make_shared<game_level>(*levelData);

  player = CreatePlayerShip();
  player->xPos = currentLevel->playerStartPosX;
  player->yPos = currentLevel->playerStartPosY;
}

game_state::game_state()
{
}
