#include "game_state.h"
#include <list>

game_state::game_state()
: running(true), screen(title)
{
  static const int fps = 60;
  timer = std::make_unique<system_timer>(fps);
}

game_events_ptr UpdateGameState(game_state& gameState, const control_state& controlState)
{
  UpdateSystemTimer(*gameState.timer);

  if( gameState.starting )
  {
    gameState.starting = false;
  }

  if( controlState.quitPress )
  {
    switch( gameState.screen )
    {
      case game_state::title:
        gameState.running = false;
        return std::make_shared<game_events>();
      case game_state::main:
        gameState.screen = game_state::title;
        break;
    }
  }

  if( gameState.screen == game_state::title )
  {
    if( controlState.startGame ){
      gameState.screen = game_state::main;
      gameState.playState = std::make_unique<play_state>(*gameState.timer, CreateInitialGameLevel());
    }
    return std::make_shared<game_events>();
  }

  if( !gameState.playState ) return std::make_shared<game_events>();

  return UpdatePlayState(*gameState.playState, controlState);
}

std::unique_ptr<game_state> CreateInitialGameState()
{
  std::unique_ptr<game_state> gameState = std::make_unique<game_state>();
  return gameState;
}
