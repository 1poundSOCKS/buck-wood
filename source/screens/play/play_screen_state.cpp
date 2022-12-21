#include "pch.h"
#include "play_screen_state.h"
#include "render.h"
#include "diagnostics.h"

void OnGamePaused(play_screen_state& screenState, const screen_input_state& inputState);
void OnGameRunning(play_screen_state& screenState, const screen_input_state& inputState);
void OnGamePlaying(play_screen_state& screenState, const screen_input_state& inputState);
bool AllLevelsAreComplete(play_screen_state& screenState);
void LoadNextLevel(play_screen_state& screenState);
bool ScreenTransitionTimeHasExpired(play_screen_state& screenState);
void SetScreenTransitionDelay(play_screen_state& screenState, int timeInSeconds);
level_control_state GetLevelControlState(const screen_input_state& inputState);

play_screen_state::play_screen_state(
  game_level_data_index::const_iterator currentLevelDataIterator, 
  game_level_data_index::const_iterator endLevelDataIterator,
  const screen_render_data& renderData,
  const bespoke_sound_data& soundData)
:
  currentLevelDataIterator(currentLevelDataIterator),
  endLevelDataIterator(endLevelDataIterator),
  renderData(renderData),
  soundData(soundData)
{
  timer.frequency = performance_counter::QueryFrequency();
  timer.initialValue = timer.currentValue = performance_counter::QueryValue();
  
  const auto& levelData = **currentLevelDataIterator;
  levelState = std::make_unique<level_state>(**currentLevelDataIterator, this->timer.frequency, renderData);
  levelStart = this->timer.initialValue;

  mode = playing;
}

void UpdateScreenState(play_screen_state& screenState, const screen_input_state& inputState)
{
  screenState.timer.currentValue = performance_counter::QueryValue();

  screenState.renderTargetMouseData = inputState.renderTargetMouseData;

  screenState.levelState->playerShot = screenState.levelState->targetShot = false;

  if( screenState.mode == play_screen_state::paused )
    OnGamePaused(screenState, inputState);
  else if( ScreenTransitionTimeHasExpired(screenState) )
    OnGameRunning(screenState, inputState);
}

void OnGamePaused(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_Q) )
  {
    screenState.continueRunning = false;
  }
  else if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    screenState.mode = play_screen_state::playing;
    screenState.pauseTotal += ( screenState.timer.currentValue - screenState.pauseStart );
  }
}

void OnGameRunning(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( screenState.mode == play_screen_state::playing )
  {
    OnGamePlaying(screenState, inputState);
  }
  else if( screenState.mode == play_screen_state::level_complete )
  {
    screenState.levelTimes.push_back(GetPlayTimeRemainingInSeconds(*screenState.levelState));
    
    if( AllLevelsAreComplete(screenState) )
    {
      screenState.mode = play_screen_state::game_complete;
      SetScreenTransitionDelay(screenState, 3);
    }
    else
    {
      LoadNextLevel(screenState);
      screenState.mode = play_screen_state::playing;
    }
  }
  else if( screenState.mode == play_screen_state::game_complete )
  {
    screenState.continueRunning = false;
  }
  else if( screenState.mode == play_screen_state::player_dead )
  {
    screenState.continueRunning = false;
  }
}

void OnGamePlaying(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    screenState.mode = play_screen_state::paused;
    screenState.pauseStart = screenState.timer.currentValue;
  }
  else
  {
    UpdateLevelState(
      *screenState.levelState, GetLevelControlState(inputState), 
      screenState.timer.currentValue - screenState.levelStart - screenState.pauseTotal);

    if( LevelTimedOut(*screenState.levelState) )
    {
      screenState.mode = play_screen_state::player_dead;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( LevelIsComplete(*screenState.levelState) )
    {
      screenState.mode = play_screen_state::level_complete;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( PlayerIsDead(*screenState.levelState) )
    {
      screenState.mode = play_screen_state::player_dead;
      SetScreenTransitionDelay(screenState, 3);
    }
  }
}

bool ScreenTransitionTimeHasExpired(play_screen_state& screenState)
{
  return screenState.timer.currentValue > screenState.transitionEnd;
}

void SetScreenTransitionDelay(play_screen_state& screenState, int timeInSeconds)
{
  screenState.transitionEnd = screenState.timer.currentValue + (timeInSeconds * screenState.timer.frequency);
}

void OnPlay(play_screen_state& screenState, const screen_input_state& inputState)
{
  if( GetPlayTimeRemaining(*screenState.levelState) > 0 )
  {
    UpdateLevelState(
      *screenState.levelState, 
      GetLevelControlState(inputState), 
      screenState.timer.currentValue - screenState.levelStart - screenState.pauseTotal);

    if( LevelIsComplete(*screenState.levelState) )
    {
      screenState.mode = play_screen_state::level_complete;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( screenState.levelState->player.state == player_ship::dead )
    {
      screenState.mode = play_screen_state::player_dead;
      SetScreenTransitionDelay(screenState, 3);
    }
  }
  else
  {
    screenState.mode = play_screen_state::player_dead;
    SetScreenTransitionDelay(screenState, 3);
  }
}

bool AllLevelsAreComplete(play_screen_state& screenState)
{
  return std::next(screenState.currentLevelDataIterator) == screenState.endLevelDataIterator ? true : false;
}

void LoadNextLevel(play_screen_state& screenState)
{
  auto nextLevel = std::next(screenState.currentLevelDataIterator);
  assert(nextLevel != screenState.endLevelDataIterator);
  screenState.currentLevelDataIterator = nextLevel;
  const auto& levelData = **screenState.currentLevelDataIterator;
  screenState.levelState = std::make_unique<level_state>(**screenState.currentLevelDataIterator, screenState.timer.frequency, screenState.renderData);
  screenState.levelStart = screenState.timer.currentValue;
  screenState.timer.initialValue = screenState.timer.initialValue;
}

bool ContinueRunning(const play_screen_state& screenState)
{
  return screenState.continueRunning;
}

level_control_state GetLevelControlState(const screen_input_state& inputState)
{
  return {
    inputState.windowData.mouse.rightButtonDown, 
    inputState.windowData.mouse.leftButtonDown, 
    inputState.renderTargetMouseData
  };
}
