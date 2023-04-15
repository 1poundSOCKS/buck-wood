#include "pch.h"
#include "play_screen_state.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"

level_control_state GetLevelControlState(const screen_input_state& inputState);

void UpdateScreenState(play_screen_state& screenState, const screen_input_state& inputState)
{
  screenState.timer.currentValue = performance_counter::QueryValue();
  screenState.renderTargetMouseData = inputState.renderTargetMouseData;
  screenState.playerShot = screenState.targetShot = false;

  if( screenState.mode == play_screen_state::paused )
  {
    screenState.OnGamePaused(inputState);
  }
  else
  {
    screenState.UpdateLevelState(inputState);

    if( screenState.ScreenTransitionTimeHasExpired() )
      screenState.OnGameRunning(inputState);
  }
}

bool ContinueRunning(const play_screen_state& screenState)
{
  return screenState.continueRunning;
}

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

void play_screen_state::OnGameRunning(const screen_input_state& inputState)
{
  if( mode == playing )
  {
    OnGamePlaying(inputState);
  }
  else if( mode == level_complete )
  {
    levelTimes.push_back(m_levelTimer->GetTimeRemainingInSeconds());
    
    if( AllLevelsAreComplete() )
    {
      mode = game_complete;
      SetScreenTransitionDelay(3);
    }
    else
    {
      LoadNextLevel();
      mode = playing;
    }
  }
  else if( mode == game_complete )
  {
    continueRunning = false;
  }
  else if( mode == player_dead )
  {
    continueRunning = false;
  }
}

void play_screen_state::OnGamePlaying(const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    mode = paused;
    pauseStart = timer.currentValue;
  }
  else
  {
    levelState->Update(timer.currentValue - levelStart - pauseTotal);

    if( m_levelTimer->HasExpired() )
    {
      mode = player_dead;
      SetScreenTransitionDelay(3);
    }
    else if( levelState->IsComplete() )
    {
      m_levelTimer->Stop();
      mode = level_complete;
      SetScreenTransitionDelay(3);
    }
    else if( player->GetState() == player_ship::dead )
    {
      mode = player_dead;
      m_levelTimer->Stop();
      SetScreenTransitionDelay(3);
    }
  }
}

void play_screen_state::OnGamePaused(const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_Q) )
  {
    continueRunning = false;
  }
  else if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    mode = playing;
    pauseTotal += ( timer.currentValue - pauseStart );
  }
}

auto play_screen_state::UpdateLevelState(const screen_input_state& inputState) -> void
{
  auto levelControlState = GetLevelControlState(inputState);
  player->SetThruster(levelControlState.thrust);
  player->SetShoot(levelControlState.shoot);

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

  levelState->Update(timer.currentValue - levelStart - pauseTotal);
}

bool play_screen_state::ScreenTransitionTimeHasExpired()
{
  return timer.currentValue > transitionEnd;
}

void play_screen_state::SetScreenTransitionDelay(int timeInSeconds)
{
  transitionEnd = timer.currentValue + (timeInSeconds * timer.frequency);
}

bool play_screen_state::AllLevelsAreComplete()
{
  return std::next(currentLevelDataIterator) == endLevelDataIterator ? true : false;
}

void play_screen_state::LoadNextLevel()
{
  auto nextLevel = std::next(currentLevelDataIterator);
  assert(nextLevel != endLevelDataIterator);
  currentLevelDataIterator = nextLevel;
  const game_level_data& gameLevelData = **currentLevelDataIterator;
  LoadLevel(gameLevelData);
  levelStart = timer.currentValue;
  timer.initialValue = timer.initialValue;
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

  screen_render_brush_selector brushes { renderData.renderBrushes };
  screen_render_text_format_selector textFormats { renderData.textFormats };

  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::transform(levelObjects.cbegin(), levelObjects.cend(), std::back_inserter(islands), [brushes](const auto& object) -> level_island
  {
    return { object, brushes };
  });
  
  std::copy(islands.cbegin(), islands.cend(), levelState->GetActiveObjectInserter());

  std::vector<target_state> targets;
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [brushes](const auto& target) -> target_state
  {
    return { target, brushes };
  });

  std::copy(targets.cbegin(), targets.cend(), levelState->GetActiveObjectInserter());

  AddPlayer(levelData.playerStartPosX, levelData.playerStartPosY);

  levelTimeLimit = levelData.timeLimitInSeconds * timer.frequency;

  m_levelTimer = std::make_unique<level_timer>(brushes, textFormats, levelData.timeLimitInSeconds);
  levelState->GetOverlayObjectInserter() = *m_levelTimer;
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

[[nodiscard]] auto play_screen_state::GetMouseDiagnostics() const -> std::wstring
{
  return std::format(L"world mouse: {:.2f}, {:.2f}", mouseX, mouseY);
}
