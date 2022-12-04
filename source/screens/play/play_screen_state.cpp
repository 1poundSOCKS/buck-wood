#include "pch.h"
#include "play_screen_state.h"
#include "render.h"
#include "game_math.h"

void OnPlay(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
void OnLevelComplete(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);

play_screen_state::play_screen_state(const system_timer& timer, game_level_data_index::const_iterator currentLevelDataIterator, game_level_data_index::const_iterator endLevelDataIterator) 
: systemTimer(timer), 
  currentLevelDataIterator(currentLevelDataIterator),
  endLevelDataIterator(endLevelDataIterator)
{
  const auto& levelData = **currentLevelDataIterator;
  levelState = std::make_unique<level_state>(**currentLevelDataIterator, systemTimer);

  levelTimer = std::make_unique<stopwatch>(systemTimer, static_cast<int>(levelData.timeLimitInSeconds), 1);
  pauseTimer = std::make_unique<stopwatch>(systemTimer);

  state = play_screen_state::state_playing;
}

void UpdateScreenState(play_screen_state& screenState, const screen_input_state& inputState, const system_timer& timer)
{
  play_screen_control_state controlState;
  ReadControlState(inputState, controlState);
  
  screenState.renderTargetMouseData = controlState.levelControlState.renderTargetMouseData;

  auto& levelState = *screenState.levelState;

  UpdateStopwatch(*screenState.levelTimer);
  UpdateStopwatch(*screenState.pauseTimer);
  UpdateStopwatch(levelState.shotTimer);

  screenState.returnToMenu = levelState.playerShot = levelState.targetShot = false;

  if( screenState.state == play_screen_state::state_paused )
  {
    if( controlState.returnToMenu )
    {
      screenState.returnToMenu = true;
      return;
    }

    if( controlState.restartPlay )
    {
      screenState.state = play_screen_state::state_playing;
      screenState.levelTimer->paused = false;
      levelState.shotTimer.paused = false;
      return;
    }
  }
  
  if( GetTicksRemaining(*screenState.pauseTimer) > 0 ) return;

  if( screenState.state == play_screen_state::state_game_complete || 
      screenState.state == play_screen_state::state_player_dead )
  {
    screenState.returnToMenu = true;
    return;
  }

  if( screenState.state == play_screen_state::state_level_complete )
  {
    OnLevelComplete(screenState, controlState, timer);
    return;
  }

  if( screenState.state == play_screen_state::state_playing )
  {
    if( controlState.pausePlay )
    {
      screenState.state = play_screen_state::state_paused;
      screenState.levelTimer->paused = true;
      levelState.shotTimer.paused = true;
      return;
    }

    screenState.levelTimer->paused = false;
    levelState.shotTimer.paused = false;
    OnPlay(screenState, controlState, timer);
    return;
  }
}

void OnPlay(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer)
{
  int64_t ticksRemaining = GetTicksRemaining(*screenState.levelTimer);

  if( ticksRemaining == 0 )
  {
    screenState.state = play_screen_state::state_player_dead;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }
  
  auto& levelState = *screenState.levelState;
  const auto& levelControlState = controlState.levelControlState;

  UpdateState(levelState, levelControlState, timer);

  if( LevelIsComplete(*screenState.levelState) )
  {
    screenState.state = play_screen_state::state_level_complete;
    screenState.levelTimer->paused = true;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }

  if( levelState.player.state == player_ship::player_state::state_dead )
  {
    screenState.state = play_screen_state::state_player_dead;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
  }
}

void OnLevelComplete(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer)
{
  float levelTimeRemaining = GetTimeRemainingInSeconds(*screenState.levelTimer);
  screenState.levelTimes.push_back(levelTimeRemaining);

  const auto nextLevel = std::next(screenState.currentLevelDataIterator);

  if( nextLevel == screenState.endLevelDataIterator )
  {
    screenState.state = play_screen_state::state_game_complete;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }

  screenState.currentLevelDataIterator = nextLevel;
  
  const auto& levelData = **screenState.currentLevelDataIterator;

  screenState.levelState = std::make_unique<level_state>(**screenState.currentLevelDataIterator, timer);

  ResetStopwatch(*screenState.levelTimer, levelData.timeLimitInSeconds);

  screenState.state = play_screen_state::state_playing;
}

bool ContinueRunning(const play_screen_state& screenState)
{
  return screenState.returnToMenu ? false : true;
}

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const play_screen_state& screenState, const play_screen_control_state& controlState)
{
  auto& levelState = *screenState.levelState;

  static wchar_t text[64];

  swprintf(text, L"world mouse X: %.1f", levelState.mouseX);
  diagnosticsData = text;

  swprintf(text, L"world mouse Y: %.1f", levelState.mouseY);
  diagnosticsData = text;

  swprintf(text, L"bullet count: %I64u", levelState.bullets.size());
  diagnosticsData = text;

  swprintf(text, L"initial ticks: %I64u", screenState.levelTimer->initialTicks);
  diagnosticsData = text;

  swprintf(text, L"end ticks: %I64u", screenState.levelTimer->endTicks);
  diagnosticsData = text;

  swprintf(text, L"current ticks: %I64u", screenState.levelTimer->currentTicks);
  diagnosticsData = text;

  int64_t ticksRemaining = GetTicksRemaining(*screenState.levelTimer);
  swprintf(text, L"remaining ticks: %I64u", ticksRemaining);
  diagnosticsData = text;
}
