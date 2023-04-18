#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"

play_screen::play_screen()
{
}

auto play_screen::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  timer.frequency = performance_counter::QueryFrequency();
  timer.initialValue = timer.currentValue = performance_counter::QueryValue();
  levelStart = this->timer.initialValue;
  currentLevelDataIterator = { global_state::firstLevelData() };
  endLevelDataIterator = { global_state::endLevelData() };
  CreateScreenRenderBrushes(renderTarget, std::back_inserter(m_renderBrushes));
  CreateScreenRenderTextFormats(dwriteFactory, std::back_inserter(m_textFormats));
  m_mouseCursor = std::make_unique<mouse_cursor>(screen_render_brush_selector { m_renderBrushes });
  LoadLevel(**currentLevelDataIterator);
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  timer.currentValue = performance_counter::QueryValue();
  renderTargetMouseData = inputState.renderTargetMouseData;
  playerShot = targetShot = false;

  UpdateMouseCursorPosition();

  if( m_levelState->GetState() == level_state::paused )
  {
    OnGamePaused(inputState);
  }
  else
  {
    UpdateLevelState(inputState);

    if( ScreenTransitionTimeHasExpired() )
      OnGameRunning(inputState);
  }
}

auto play_screen::RenderTo(ID2D1RenderTarget* renderTarget) const -> void
{
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  m_levelObjectContainer->RenderTo(renderTarget, m_viewTransform);
}

auto play_screen::PlaySoundEffects() const -> void
{
  const auto soundBuffers = global_sound_buffer_selector { sound_data::soundBuffers() };

  if( m_levelState->GetState() == level_state::playing )
  {
    PlaySoundEffects(soundBuffers);
  }
  else
  {
    StopSoundBufferPlay(soundBuffers[menu_theme]);
    StopSoundBufferPlay(soundBuffers[thrust]);
  }
}

[[nodiscard]] auto play_screen::ContinueRunning() const -> bool
{
  return continueRunning;
}

auto play_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
  diagnosticsDataInserter = GetMouseDiagnostics();
}

void play_screen::OnGameRunning(const screen_input_state& inputState)
{
  if( m_levelState->GetState() == level_state::playing )
  {
    OnGamePlaying(inputState);
  }
  else if( m_levelState->GetState() == level_state::complete )
  {
    levelTimes.push_back(m_levelTimer->GetTimeRemainingInSeconds());
    
    if( AllLevelsAreComplete() )
    {
      m_gameComplete = true;
      continueRunning = false;
      SetScreenTransitionDelay(3);
    }
    else
    {
      LoadNextLevel();
    }
  }
  else if( m_gameComplete )
  {
    continueRunning = false;
  }
  else if( m_levelState->GetState() == level_state::dead )
  {
    continueRunning = false;
  }
}

void play_screen::OnGamePlaying(const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    m_levelState->SetState(level_state::paused);
    pauseStart = timer.currentValue;
  }
  else
  {
    m_levelObjectContainer->Update(timer.currentValue - levelStart - pauseTotal);

    if( m_levelTimer->HasExpired() )
    {
      m_levelState->SetState(level_state::dead);
      SetScreenTransitionDelay(3);
    }
    else if( m_levelObjectContainer->IsComplete() )
    {
      m_levelState->SetState(level_state::complete);
      m_levelTimer->Stop();
      SetScreenTransitionDelay(3);
    }
    else if( player->GetState() == player_ship::dead )
    {
      m_levelState->SetState(level_state::dead);
      m_levelTimer->Stop();
      SetScreenTransitionDelay(3);
    }
  }
}

void play_screen::OnGamePaused(const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_Q) )
  {
    continueRunning = false;
  }
  else if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    m_levelState->SetState(level_state::playing);
    pauseTotal += ( timer.currentValue - pauseStart );
  }
}

auto play_screen::UpdateMouseCursorPosition() -> void
{
  m_mouseCursor->SetPosition(renderTargetMouseData.x, renderTargetMouseData.y);
}

auto play_screen::UpdateLevelState(const screen_input_state& inputState) -> void
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

  m_levelObjectContainer->Update(timer.currentValue - levelStart - pauseTotal);
}

bool play_screen::ScreenTransitionTimeHasExpired()
{
  return timer.currentValue > transitionEnd;
}

void play_screen::SetScreenTransitionDelay(int timeInSeconds)
{
  transitionEnd = timer.currentValue + (timeInSeconds * timer.frequency);
}

bool play_screen::AllLevelsAreComplete()
{
  return std::next(currentLevelDataIterator) == endLevelDataIterator ? true : false;
}

void play_screen::LoadNextLevel()
{
  auto nextLevel = std::next(currentLevelDataIterator);
  assert(nextLevel != endLevelDataIterator);
  currentLevelDataIterator = nextLevel;
  const game_level_data& gameLevelData = **currentLevelDataIterator;
  LoadLevel(gameLevelData);
  levelStart = timer.currentValue;
  timer.initialValue = timer.initialValue;
}

level_control_state play_screen::GetLevelControlState(const screen_input_state& inputState)
{
  return {
    inputState.windowData.mouse.rightButtonDown, 
    inputState.windowData.mouse.leftButtonDown, 
    inputState.renderTargetMouseData
  };
}

auto play_screen::LoadLevel(const game_level_data& levelData) -> void
{
  m_levelObjectContainer = std::make_unique<level_object_container>(timer.frequency);

  m_levelObjectContainer->GetOverlayObjectInserter() = *m_mouseCursor;

  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::transform(levelObjects.cbegin(), levelObjects.cend(), std::back_inserter(islands), [this](const auto& object) -> level_island
  {
    return { object, screen_render_brush_selector { m_renderBrushes } };
  });
  
  std::copy(islands.cbegin(), islands.cend(), m_levelObjectContainer->GetActiveObjectInserter());

  std::vector<target_state> targets;
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [this](const auto& target) -> target_state
  {
    return { target, screen_render_brush_selector { m_renderBrushes } };
  });

  std::copy(targets.cbegin(), targets.cend(), m_levelObjectContainer->GetActiveObjectInserter());

  AddPlayer(levelData.playerStartPosX, levelData.playerStartPosY);

  m_levelState = std::make_unique<level_state>( screen_render_brush_selector { m_renderBrushes }, screen_render_text_format_selector { m_textFormats } );
  m_levelObjectContainer->GetOverlayObjectInserter() = *m_levelState;

  levelTimeLimit = levelData.timeLimitInSeconds * timer.frequency;

  m_levelTimer = std::make_unique<level_timer>(screen_render_brush_selector { m_renderBrushes }, screen_render_text_format_selector { m_textFormats } ,
    levelData.timeLimitInSeconds);
  
  m_levelObjectContainer->GetOverlayObjectInserter() = *m_levelTimer;
}

auto play_screen::AddPlayer(float x, float y) -> void
{
  screen_render_brush_selector brushSelector { m_renderBrushes };
  player = std::make_unique<player_ship>(timer.frequency, brushSelector);
  player->SetPosition(x, y);

  player->SetEventShot([this](float x, float y, float angle) -> void
  {
    screen_render_brush_selector brushSelector { m_renderBrushes };
    bullet newBullet { x, y, angle, brushSelector };
    auto activeObjectInserter = this->m_levelObjectContainer->GetActiveObjectInserter();
    activeObjectInserter = newBullet;
    playerShot = true;
  });

  player->SetEventDied([this](float x, float y) -> void
  {
  });

  auto activeObjectInserter = m_levelObjectContainer->GetActiveObjectInserter();
  activeObjectInserter = *player;
}

[[nodiscard]] auto play_screen::CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale) -> D2D1::Matrix3x2F
{
  m_viewTransform = CreateGameLevelTransform(player->GetXPos(), player->GetYPos(), renderScale, renderTargetSize.width, renderTargetSize.height);
  return m_viewTransform;
}

auto play_screen::PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void
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

[[nodiscard]] auto play_screen::GetMouseDiagnostics() const -> std::wstring
{
  return std::format(L"world mouse: {:.2f}, {:.2f}", mouseX, mouseY);
}
