#include "pch.h"
#include "play_screen_state.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"

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
  const sound_data& soundData)
:
  currentLevelDataIterator(currentLevelDataIterator),
  endLevelDataIterator(endLevelDataIterator),
  renderData(renderData),
  soundData(soundData)
{
  timer.frequency = performance_counter::QueryFrequency();
  timer.initialValue = timer.currentValue = performance_counter::QueryValue();
  levelStart = this->timer.initialValue;

  LoadLevel(**currentLevelDataIterator);

  mode = playing;
}

void UpdateScreenState(play_screen_state& screenState, const screen_input_state& inputState)
{
  screenState.timer.currentValue = performance_counter::QueryValue();
  screenState.renderTargetMouseData = inputState.renderTargetMouseData;
  screenState.playerShot = screenState.targetShot = false;

  if( screenState.mode == play_screen_state::paused )
  {
    OnGamePaused(screenState, inputState);
  }
  else if( ScreenTransitionTimeHasExpired(screenState) )
  {
    screenState.UpdateLevelState(inputState);
    OnGameRunning(screenState, inputState);
  }
  else
  {
    screenState.UpdateLevelState(inputState);
  }
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

auto play_screen_state::UpdateLevelState(const screen_input_state& inputState) -> void
{
  auto levelControlState = GetLevelControlState(inputState);
  player->SetThruster(levelControlState.thrust);
  player->SetShoot(levelControlState.shoot);

  levelState->Update(timer.currentValue - levelStart - pauseTotal);
  
  auto invertedViewTransform = CreateViewTransform(levelControlState.renderTargetMouseData.size);
  if( invertedViewTransform.Invert() )
  {
    D2D1_POINT_2F inPoint { levelControlState.renderTargetMouseData.x, levelControlState.renderTargetMouseData.y };
    auto outPoint = invertedViewTransform.TransformPoint(inPoint);

    mouseX = outPoint.x;
    mouseY = outPoint.y;

    auto playerAngle = CalculateAngle(player->GetXPos(), player->GetYPos(), outPoint.x, outPoint.y);
    player->SetAngle(playerAngle);

    D2D1_POINT_2F screenTopLeft { 0, 0 };
    D2D1_POINT_2F screenBottomRight { levelControlState.renderTargetMouseData.size.width - 1, levelControlState.renderTargetMouseData.size.height - 1 };
    auto viewTopLeft = invertedViewTransform.TransformPoint(screenTopLeft);
    auto viewBottomRight = invertedViewTransform.TransformPoint(screenBottomRight);
    m_viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
    m_viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
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
    screenState.levelTimes.push_back(screenState.GetPlayTimeRemainingInSeconds());
    
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
    screenState.levelState->Update(screenState.timer.currentValue - screenState.levelStart - screenState.pauseTotal);

    if( screenState.TimedOut() )
    {
      screenState.mode = play_screen_state::player_dead;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( screenState.levelState->IsComplete() )
    {
      screenState.mode = play_screen_state::level_complete;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( screenState.PlayerIsDead() )
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
  if( screenState.GetPlayTimeRemaining() > 0 )
  {
    screenState.levelState->Update(screenState.timer.currentValue - screenState.levelStart - screenState.pauseTotal);

    if( screenState.levelState->IsComplete() )
    {
      screenState.mode = play_screen_state::level_complete;
      SetScreenTransitionDelay(screenState, 3);
    }
    else if( screenState.PlayerIsDead() )
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
  const game_level_data& gameLevelData = **screenState.currentLevelDataIterator;
  screenState.LoadLevel(gameLevelData);
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

auto play_screen_state::LoadLevel(const game_level_data& levelData) -> void
{
  levelState = std::make_unique<level_state>(timer.frequency);

  screen_render_brush_selector brushSelector { renderData.renderBrushes };

  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::transform(levelObjects.cbegin(), levelObjects.cend(), std::back_inserter(islands), [brushSelector](const auto& object) -> level_island
  {
    return { object, brushSelector };
  });
  
  std::copy(islands.cbegin(), islands.cend(), levelState->GetActiveObjectInserter());

  std::vector<target_state> targets;
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [brushSelector](const auto& target) -> target_state
  {
    return { target, brushSelector };
  });

  std::copy(targets.cbegin(), targets.cend(), levelState->GetActiveObjectInserter());

  AddPlayer(levelData.playerStartPosX, levelData.playerStartPosY);

  levelTimeLimit = levelData.timeLimitInSeconds * timer.frequency;
}

auto play_screen_state::AddPlayer(float x, float y) -> void
{
  screen_render_brush_selector brushSelector { renderData.renderBrushes };
  player = std::make_unique<player_ship>(timer.frequency, brushSelector);
  player->SetPosition(x, y);

  player->SetEventShot([this](float x, float y, float angle) -> void
  {
    screen_render_brush_selector renderBrushSelector { renderData.renderBrushes };
    bullet newBullet { x, y, angle, renderBrushSelector };
    auto activeObjectInserter = this->levelState->GetActiveObjectInserter();
    activeObjectInserter = newBullet;
    playerShot = true;
  });

  player->SetEventDied([this](float x, float y) -> void
  {
  });

  auto activeObjectInserter = levelState->GetActiveObjectInserter();
  activeObjectInserter = *player;
}

[[nodiscard]] auto play_screen_state::CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale) -> D2D1::Matrix3x2F
{
  m_viewTransform = CreateGameLevelTransform(player->GetXPos(), player->GetYPos(), renderScale, renderTargetSize.width, renderTargetSize.height);
  return m_viewTransform;
}

[[nodiscard]] auto play_screen_state::PlayerIsDead() -> bool
{
  return player->GetState() == player_ship::dead;
}

auto play_screen_state::PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void
{
  PlaySoundBuffer(soundBuffers[menu_theme], true);

  if( player->ThrusterOn() )
    PlaySoundBuffer(soundBuffers[thrust], true);
  else
    StopSoundBufferPlay(soundBuffers[thrust]);

  if( playerShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[shoot]);
  }

  if( targetShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[target_activated]);
  }
}

[[nodiscard]] auto play_screen_state::TimedOut() const -> bool
{
  return GetPlayTimeRemaining() > 0 ? false : true;
}

[[nodiscard]] auto play_screen_state::GetPlayTimeRemaining() const -> int64_t
{
  int64_t playTimeRemaining = levelTimeLimit - (timer.currentValue - timer.initialValue) - pauseTotal;
  return max(0, playTimeRemaining);
}

[[nodiscard]] auto play_screen_state::GetPlayTimeRemainingInSeconds() const -> float
{
  return static_cast<float>(GetPlayTimeRemaining()) / static_cast<float>(timer.frequency);
}

[[nodiscard]] auto play_screen_state::GetMouseDiagnostics() const -> std::wstring
{
  return std::format(L"world mouse: {:.2f}, {:.2f}", mouseX, mouseY);
}
