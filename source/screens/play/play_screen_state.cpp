#include "pch.h"
#include "play_screen_state.h"
#include "render.h"
#include "game_math.h"
#include "diagnostics.h"

void OnGamePaused(play_screen_state& screenState, const screen_input_state& inputState);
void OnGameRunning(play_screen_state& screenState, const screen_input_state& inputState);
void OnPlay(play_screen_state& screenState, const screen_input_state& inputState);
void OnLevelComplete(play_screen_state& screenState);
bool ScreenTransitionTimeHasExpired(play_screen_state& screenState);
void SetScreenTransitionDelay(play_screen_state& screenState, int timeInSeconds);
level_control_state GetLevelControlState(const screen_input_state& inputState);

play_screen_state::play_screen_state(
  game_level_data_index::const_iterator currentLevelDataIterator, 
  game_level_data_index::const_iterator endLevelDataIterator) 
:
  currentLevelDataIterator(currentLevelDataIterator),
  endLevelDataIterator(endLevelDataIterator)
{
  timer.frequency = performance_counter::QueryFrequency();
  timer.initialValue = timer.currentValue = performance_counter::QueryValue();
  
  const auto& levelData = **currentLevelDataIterator;
  levelState = std::make_unique<level_state>(**currentLevelDataIterator, this->timer.frequency);
  levelStartCount = this->timer.initialValue;

  state = play_screen_state::state_playing;
}

void UpdateScreenState(play_screen_state& screenState, const screen_input_state& inputState)
{
  screenState.timer.currentValue = performance_counter::QueryValue();

  screenState.renderTargetMouseData = inputState.renderTargetMouseData;

  screenState.returnToMenu = screenState.levelState->playerShot = screenState.levelState->targetShot = false;

  if( screenState.state == play_screen_state::state_paused )
    OnGamePaused(screenState, inputState);
  else if( ScreenTransitionTimeHasExpired(screenState) )
    OnGameRunning(screenState, inputState);
}

void OnGamePaused(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_Q) )
  {
    screenState.returnToMenu = true;
  }
  else if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    screenState.state = play_screen_state::state_playing;
    screenState.pauseTotalCount += ( screenState.timer.currentValue - screenState.pauseStartCount );
  }
}

void OnGameRunning(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( screenState.state == play_screen_state::state_game_complete || 
      screenState.state == play_screen_state::state_player_dead )
  {
    screenState.returnToMenu = true;
  }
  else if( screenState.state == play_screen_state::state_level_complete )
  {
    OnLevelComplete(screenState);
  }
  else if( screenState.state == play_screen_state::state_playing )
  {
    if( KeyPressed(inputState, DIK_ESCAPE) )
    {
      screenState.state = play_screen_state::state_paused;
      screenState.pauseStartCount = screenState.timer.currentValue;
    }
    else
      OnPlay(screenState, inputState);
  }
}

bool ScreenTransitionTimeHasExpired(play_screen_state& screenState)
{
  return screenState.timer.currentValue > screenState.transitionEndCount;
}

void SetScreenTransitionDelay(play_screen_state& screenState, int timeInSeconds)
{
  screenState.transitionEndCount = screenState.timer.currentValue + (timeInSeconds * screenState.timer.frequency);
}

void OnPlay(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( GetPlayTimeRemaining(*screenState.levelState) > 0 )
  {
    UpdateLevelState(
      *screenState.levelState, 
      GetLevelControlState(inputState), 
      screenState.timer.currentValue - screenState.levelStartCount - screenState.pauseTotalCount);

    if( LevelIsComplete(*screenState.levelState) )
    {
      screenState.state = play_screen_state::state_level_complete;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( screenState.levelState->player.state == player_ship::dead )
    {
      screenState.state = play_screen_state::state_player_dead;
      SetScreenTransitionDelay(screenState, 3);
    }
  }
  else
  {
    screenState.state = play_screen_state::state_player_dead;
    SetScreenTransitionDelay(screenState, 3);
  }
}

void OnLevelComplete(play_screen_state& screenState)
{
  float levelTimeRemaining = GetPlayTimeRemainingInSeconds(*screenState.levelState);
  screenState.levelTimes.push_back(levelTimeRemaining);

  auto nextLevel = std::next(screenState.currentLevelDataIterator);

  if( nextLevel == screenState.endLevelDataIterator )
  {
    screenState.state = play_screen_state::state_game_complete;
  }
  else
  {
    screenState.currentLevelDataIterator = nextLevel;
    const auto& levelData = **screenState.currentLevelDataIterator;
    screenState.levelState = std::make_unique<level_state>(**screenState.currentLevelDataIterator, screenState.timer.frequency);
    screenState.levelStartCount = screenState.timer.currentValue;
    screenState.timer.initialValue = screenState.timer.initialValue;
    screenState.state = play_screen_state::state_playing;
  }
}

bool ContinueRunning(const play_screen_state& screenState)
{
  return screenState.returnToMenu ? false : true;
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const play_screen_state& screenState)
{
  diagnosticsData = std::format(L"world mouse: {:.2f}, {:.2f}", screenState.levelState->mouseX, screenState.levelState->mouseY);
}

level_control_state GetLevelControlState(const screen_input_state& inputState)
{
  return {
    inputState.windowData.mouse.rightButtonDown, 
    inputState.windowData.mouse.leftButtonDown, 
    inputState.renderTargetMouseData
  };
}
