#include "game_state.h"
#include <list>
#include "play_state.h"

game_state::game_state()
{
  gameData = std::make_shared<game_data>();
  gameData->push_back(CreateFirstGameLevelData());
  gameData->push_back(CreateSecondGameLevelData());
}

game_events_ptr UpdateGameState(game_state& gameState, const control_state& controlState, const system_timer& timer)
{
  gameState.renderTargetMouseX = controlState.renderTargetMouseX;
  gameState.renderTargetMouseY = controlState.renderTargetMouseY;

  if( gameState.starting )
  {
    gameState.starting = false;
  }

  switch( gameState.screen )
  {
  case game_state::screen_play:
    return UpdatePlayState(gameState, controlState, timer);

  case game_state::screen_title:
    
    if( controlState.quitPress )
    {
      gameState.running = false;
      return std::make_shared<game_events>();
    }

    if( controlState.startGame )
    {
      gameState.screen = game_state::screen_play;
      gameState.playState = std::make_unique<play_state>(timer, gameState.gameData);
      return std::make_shared<game_events>();
    }

    if( controlState.functionKey_1 )
    {
      gameState.screen = game_state::screen_level_editor;
      // gameState.levelEditorState = std::make_unique<level_editor_state>(gameState.firstLevel);
      return std::make_shared<game_events>();
    }

    return std::make_shared<game_events>();

  case game_state::screen_level_editor:
    if( controlState.quitPress )
    {
      gameState.screen = game_state::screen_title;
      return std::make_shared<game_events>();
    }

    return UpdateLevelEditorState(*gameState.levelEditorState, controlState);
  }

  return std::make_shared<game_events>();
}

std::unique_ptr<game_state> CreateInitialGameState()
{
  std::unique_ptr<game_state> gameState = std::make_unique<game_state>();
  return gameState;
}
