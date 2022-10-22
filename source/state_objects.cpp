#include "state_objects.h"

play_state::play_state()
{
}

game_state::game_state()
{
  gameData = std::make_shared<game_data>();
  gameData->push_back(CreateFirstGameLevelData());
  gameData->push_back(CreateSecondGameLevelData());

  playState = std::make_unique<play_state>();
}
