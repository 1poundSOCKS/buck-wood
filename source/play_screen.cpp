#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "game_math.h"

void OnPlay(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
void OnLevelComplete(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes);

play_screen_sounds::play_screen_sounds(const sound_buffers& soundBuffers)
: thrust(soundBuffers.thrust),
  shoot(soundBuffers.shoot),
  targetActivated(soundBuffers.targetActivated)
{
}

play_screen_state::play_screen_state(const system_timer& timer, const global_state& globalState) 
: systemTimer(timer), 
  globalState(globalState),
  renderBrushes(globalState.renderBrushes),
  sounds(globalState.soundBuffers)
{
  currentLevelDataIterator = globalState.gameLevelDataIndex->gameLevelData.begin();

  const auto& levelData = **currentLevelDataIterator;
  levelState = std::make_unique<level_state>(**currentLevelDataIterator, systemTimer);

  levelTimer = std::make_unique<stopwatch>(systemTimer, static_cast<int>(levelData.timeLimitInSeconds), 1);
  pauseTimer = std::make_unique<stopwatch>(systemTimer);

  levelTimes.reserve(globalState.gameLevelDataIndex->gameLevelData.size());

  state = play_screen_state::state_playing;
}

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState)
{
  controlState.pausePlay = baseControlState.escapeKeyPress;
  controlState.returnToMenu = baseControlState.escapeKeyPress;
  controlState.restartPlay = baseControlState.spacebarKeyPress;
  RefreshControlState(controlState.levelControlState, baseControlState);
}

void UpdateScreenState(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer)
{
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

  if( nextLevel == screenState.globalState.gameLevelDataIndex->gameLevelData.end() )
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

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const play_screen_state& screenState)
{
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderFrame(renderTarget, renderBrushSelector, *screenState.levelState, screenState.renderBrushes);

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  switch( screenState.state )
  {
    case play_screen_state::state_paused:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"PAUSED", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_level_complete:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"LEVEL COMPLETE", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_game_complete:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], GetGameCompleteMsg(screenState.levelTimes), DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case play_screen_state::state_player_dead:
      RenderText(renderTarget, renderBrushSelector[cyan], textFormatSelector[diagnostics], L"DEAD", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
  }

  float levelTimeRemaining = GetTimeRemainingInSeconds(*screenState.levelTimer);
  static wchar_t timerText[64];
  swprintf(timerText, L"%.2f", levelTimeRemaining);
  RenderText(renderTarget, renderBrushSelector[yellow], textFormatSelector[diagnostics], timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
}

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes)
{
  std::wstring msg = L"";

  for( auto levelTime: levelTimes )
  {
    static wchar_t text[64];
    swprintf(text, L"%.2f\n", levelTime);
    msg += text;
  }

  return msg;
}

screen_status GetScreenStatus(const play_screen_state& screenState)
{
  if( screenState.returnToMenu ) return screen_closed;
  else return screen_active;
}

void UpdateGlobalState(global_state& globalState, const play_screen_state& screenState)
{
  globalState.currentScreenId = screen_main_menu;
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

void PlaySoundEffects(const play_screen_state& screenState)
{
  auto& levelState = *screenState.levelState;
  const auto& sounds = screenState.sounds;

  if( levelState.playerShot ) sounds.shoot.Play();
  if( levelState.targetShot ) sounds.targetActivated.Play();
  if( levelState.player.thrusterOn && screenState.state == play_screen_state::state_playing ) sounds.thrust.PlayOnLoop();
  else sounds.thrust.Stop();
}
