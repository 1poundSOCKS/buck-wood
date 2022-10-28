#include "play_screen.h"
#include "render.h"
#include "game_math.h"

const float gameSpeedMultiplier = 2.0f;

void RenderGamePaused(const d2d_frame& frame, play_screen_state& playState);
void RenderLevelComplete(const d2d_frame& frame, play_screen_state& playState);
void RenderGameComplete(const d2d_frame& frame, play_screen_state& playState);
void RenderPlayerDead(const d2d_frame& frame, play_screen_state& playState);
void OnPlay(play_screen_state& playState, const control_state& controlState, const system_timer& timer);
void OnLevelComplete(play_screen_state& playState, const control_state& controlState, const system_timer& timer);
void UpdatePlayer(play_screen_state& playState, const control_state& controlState, const system_timer& timer);
void UpdateBullets(play_screen_state& playState, const control_state& controlState, const system_timer& timer);
bool LevelIsComplete(const play_screen_state& playState);
void SetPauseTimer(play_screen_state& playState, const system_timer& timer, float timerInSeconds);
bool TimerExpired(play_screen_state& playState, const system_timer& timer);

const int play_screen_state::shotTimeNumerator;
const int play_screen_state::shotTimeDenominator;

play_screen_state::play_screen_state(const system_timer& systemTimer, const game_level_data_index& gameLevelDataIndex) 
: systemTimer(systemTimer), gameLevelDataIndex(gameLevelDataIndex), mouseCursor(std::make_unique<mouse_cursor>())
{
  state = play_screen_state::state_playing;
  
  currentLevelDataIterator = gameLevelDataIndex.begin();
  const auto& levelData = *currentLevelDataIterator;
  currentLevel = std::make_unique<game_level>(*levelData);

  levelTimer = std::make_unique<stopwatch>(systemTimer, static_cast<int>(currentLevel->timeLimitInSeconds), 1);

  player = CreatePlayerShip();
  player->xPos = currentLevel->playerStartPosX;
  player->yPos = currentLevel->playerStartPosY;

  shotTimer = std::make_unique<stopwatch>(systemTimer, shotTimeNumerator, shotTimeDenominator);
  shotTimer->paused = false;
}

void RenderFrame(const d2d_frame& frame, play_screen_state& playState)
{
#ifdef EXPERIMENTAL
  float absVelocityX = fabsf(playState.player->xVelocity);
  float absVelocityY = fabsf(playState.player->yVelocity);
  float velocity = sqrt( absVelocityX * absVelocityX + absVelocityY * absVelocityY );
  const float renderScale = 1.5f - velocity / 500;
#else
  const float renderScale = 1.0f;
#endif

  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  auto& currentLevel = *playState.currentLevel;

  auto renderTargetSize = frame.renderTarget->GetSize();

  auto levelTransform = D2D1::Matrix3x2F::Translation(-playState.player->xPos, -playState.player->yPos) * 
    D2D1::Matrix3x2F::Scale(renderScale, renderScale) *
    D2D1::Matrix3x2F::Translation(renderTargetSize.width / 2, renderTargetSize.height / 2);

  RenderLevel(currentLevel, frame, levelTransform);
  RenderPlayer(*playState.player, frame, levelTransform);

  for( const std::unique_ptr<bullet>& bullet : playState.bullets )
  {
    RenderBullet(*bullet, frame, levelTransform);
  }

  switch( playState.state )
  {
    case play_screen_state::state_paused:
      RenderGamePaused(frame, playState);
      break;
    case play_screen_state::state_level_complete:
      RenderLevelComplete(frame, playState);
      break;
    case play_screen_state::state_game_complete:
      RenderGameComplete(frame, playState);
      break;
    case play_screen_state::state_player_dead:
      RenderPlayerDead(frame, playState);
      break;
  }

  RenderTimer(frame, playState.levelTimeRemaining);

  RenderMouseCursor(frame, *playState.mouseCursor);

  if( levelTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = frame.renderTargetMouseX;
    inPoint.y = frame.renderTargetMouseY;
    auto outPoint = levelTransform.TransformPoint(inPoint);
    playState.levelMouseX = outPoint.x;
    playState.levelMouseY = outPoint.y;
  }
}

void RenderGamePaused(const d2d_frame& frame, play_screen_state& playState)
{
  std::wstring text = L"PAUSED";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
}

void RenderLevelComplete(const d2d_frame& frame, play_screen_state& playState)
{
  std::wstring text = L"LEVEL COMPLETE";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
}

void RenderGameComplete(const d2d_frame& frame, play_screen_state& playState)
{
  std::wstring text = L"F*CK YEAH";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
}

void RenderPlayerDead(const d2d_frame& frame, play_screen_state& playState)
{
  std::wstring text = L"YOU LOSE";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
}

void UpdateState(play_screen_state& playState, const control_state& controlState, const system_timer& timer)
{
  UpdateStopwatch(*playState.levelTimer);
  UpdateStopwatch(*playState.shotTimer);

  playState.returnToMenu = playState.playerShot = playState.targetShot = false;

  if( playState.state == play_screen_state::state_paused )
  {
    if( controlState.quitPress )
    {
      playState.returnToMenu = true;
      return;
    }

    if( controlState.startGame )
    {
      playState.state = play_screen_state::state_playing;
      playState.levelTimer->paused = false;
      playState.shotTimer->paused = false;
      return;
    }
  }
  
  if( !TimerExpired(playState, timer) ) return;

  if( playState.state == play_screen_state::state_playing )
  {
    if( controlState.quitPress )
    {
      playState.state = play_screen_state::state_paused;
      playState.levelTimer->paused = true;
      playState.shotTimer->paused = true;
      return;
    }

    playState.levelTimer->paused = false;
    playState.shotTimer->paused = false;
    OnPlay(playState, controlState, timer);
    return;
  }

  if( playState.state == play_screen_state::state_game_complete || 
      playState.state == play_screen_state::state_player_dead )
  {
    playState.returnToMenu = true;
    return;
  }

  if( playState.state == play_screen_state::state_level_complete )
  {
    OnLevelComplete(playState, controlState, timer);
    return;
  }
}

void OnPlay(play_screen_state& playState, const control_state& controlState, const system_timer& timer)
{
  int64_t ticksRemaining = GetTicksRemaining(*playState.levelTimer);
   playState.levelTimeRemaining = GetElapsedTimeInSeconds(ticksRemaining, playState.levelTimer->systemTimer.ticksPerSecond);

  if( ticksRemaining == 0 )
  {
    playState.state = play_screen_state::state_player_dead;
    SetPauseTimer(playState, timer, 3);
    return;
  }
  
  UpdatePlayer(playState, controlState, timer);
  UpdateBullets(playState, controlState, timer);

  if( LevelIsComplete(playState) )
  {
    playState.state = play_screen_state::state_level_complete;
    SetPauseTimer(playState, timer, 3);
    return;
  }

  std::list<game_point> transformedPoints;
  TransformPlayerShip(*playState.player, transformedPoints);

  if( !PointsInside(transformedPoints, *playState.currentLevel->boundary) )
  {
    playState.state = play_screen_state::state_player_dead;
    SetPauseTimer(playState, timer, 3);
    return;
  }
  
  for( const auto& shape : playState.currentLevel->objects)
  {
    if( PointInside(transformedPoints, *shape) )
    {
      playState.state = play_screen_state::state_player_dead;
      SetPauseTimer(playState, timer, 3);
      return;
    }
  }
}

void OnLevelComplete(play_screen_state& playState, const control_state& controlState, const system_timer& timer)
{
  playState.currentLevelDataIterator++;
  
  if( playState.currentLevelDataIterator == playState.gameLevelDataIndex.end() )
  {
    playState.state = play_screen_state::state_game_complete;
    SetPauseTimer(playState, timer, 3);
    return;
  }

  playState.lastShotTicks = timer.totalTicks;
  const auto& nextLevelData = *playState.currentLevelDataIterator;
  playState.currentLevel = std::make_unique<game_level>(*nextLevelData);

  ResetStopwatch(*playState.levelTimer, playState.currentLevel->timeLimitInSeconds);

  playState.player = CreatePlayerShip();
  playState.player->xPos = playState.currentLevel->playerStartPosX;
  playState.player->yPos = playState.currentLevel->playerStartPosY;

  playState.bullets.clear();
  playState.state = play_screen_state::state_playing;
}

bool LevelIsComplete(const play_screen_state& playState)
{
  int activatedTargetCount = 0;
  for( const auto& target: playState.currentLevel->targets )
  {
    if( target->state == target::ACTIVATED ) activatedTargetCount++;
  }

  return activatedTargetCount == playState.currentLevel->targets.size();
}

void SetPauseTimer(play_screen_state& playState, const system_timer& timer, float timerInSeconds)
{
  playState.pauseTickCount = timer.totalTicks;
  playState.pauseTimeInSeconds = 3;
}

bool TimerExpired(play_screen_state& playState, const system_timer& timer)
{
  float timeInSeconds = GetElapsedTimeInSeconds(playState.pauseTickCount, timer.totalTicks, timer.ticksPerSecond);
  return timeInSeconds >= playState.pauseTimeInSeconds;
}

void UpdatePlayer(play_screen_state& playState, const control_state& controlState, const system_timer& timer)
{
  static const float forceOfGravity = 20.0f;
  static const float playerThrust = 100.0f;
#ifdef USE_KEYBOARDFORSPIN
  static const float rotationSpeed = 150.0f;
#endif

  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState.accelerate )
  {
    playState.player->thrusterOn = true;
    forceX += playerThrust * sin(DEGTORAD(playState.player->angle));
    forceY -= playerThrust * cos(DEGTORAD(playState.player->angle));
  }
  else
  {
    playState.player->thrusterOn = false;
  }
  
#ifdef USE_KEYBOARDFORSPIN
  float spin = 0.0f;
  if( controlState.left ) spin = -rotationSpeed;
  else if( controlState.right ) spin = rotationSpeed;
#endif
  
  playState.player->xVelocity += forceX * gameUpdateInterval;
  playState.player->yVelocity += forceY * gameUpdateInterval;
  playState.player->xPos += playState.player->xVelocity * gameUpdateInterval;
  playState.player->yPos += playState.player->yVelocity * gameUpdateInterval;
#ifdef USE_KEYBOARDFORSPIN
  playState.player->angle += spin * gameUpdateInterval;
#else
  playState.player->angle = CalculateAngle(playState.player->xPos, playState.player->yPos, playState.levelMouseX, playState.levelMouseY);
#endif
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  float cx = bullet->xPos - bullet->startX;
  float cy = bullet->yPos - bullet->startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet->range || bullet->outsideLevel;
}

void UpdateBullets(play_screen_state& playState, const control_state& controlState, const system_timer& timer)
{
  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  if( controlState.shoot )
  {
    if( GetTicksRemaining(*playState.shotTimer) == 0 )
    {
      static const float bulletSpeed = 200.0f * gameUpdateInterval;
      static const float bulletRange = 2000.0f;

      std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(playState.player->xPos, playState.player->yPos, bulletRange);
      float angle = CalculateAngle(playState.player->xPos, playState.player->yPos, playState.levelMouseX, playState.levelMouseY);
      newBullet->angle = angle;
      newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
      newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
      playState.bullets.push_front(std::move(newBullet));

      playState.playerShot = true;
      // ResetStopwatch(*playState.shotTimer, 1, 60);
      ResetStopwatch(*playState.shotTimer);
      playState.shotTimer->paused = false;
    }
  }

  for(const auto& bullet : playState.bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
    bullet->outsideLevel = !PointInside(bulletPoint, *playState.currentLevel->boundary);

    for( const auto& shape : playState.currentLevel->objects)
    {
      if( PointInside(bulletPoint, *shape) ) bullet->outsideLevel = true;
    }

    for( const auto& target: playState.currentLevel->targets )
    {
      if( PointInside(bulletPoint, target->shape) )
      {
        bullet->outsideLevel = true;

        if( target->state == target::DEACTIVATED )
        {
          target->state = target::ACTIVATED;
          playState.targetShot = true;
        }
      }
    }
  }
  
  playState.bullets.remove_if(BulletHasExpired);
}

void UpdateSound(const sound_buffers& soundBuffers, const play_screen_state& playState)
{
  DWORD bufferStatus = 0;

  if( SUCCEEDED(soundBuffers.menuTheme->buffer->GetStatus(&bufferStatus)) )
  {
    if( bufferStatus & DSBSTATUS_PLAYING ) soundBuffers.menuTheme->buffer->Stop();
  }

  if( SUCCEEDED(soundBuffers.thrust->buffer->GetStatus(&bufferStatus)) )
  {
    if( bufferStatus & DSBSTATUS_PLAYING )
    {
      if( !playState.player->thrusterOn ||
          playState.state == play_screen_state::state_paused ||
          playState.state == play_screen_state::state_player_dead ||
          playState.state == play_screen_state::state_game_complete ||
          playState.state == play_screen_state::state_level_complete )
      {
        soundBuffers.thrust->buffer->Stop();
      }
    }
    else
    {
      if( playState.player->thrusterOn )
      {
        soundBuffers.thrust->buffer->SetCurrentPosition(0);
        soundBuffers.thrust->buffer->Play(0, 0, DSBPLAY_LOOPING);
      }
    }
  }

  if( playState.playerShot )
  {
    soundBuffers.shoot->buffer->SetCurrentPosition(0);
    soundBuffers.shoot->buffer->Play(0, 0, 0);
  }

  if( playState.targetShot )
  {
    soundBuffers.targetActivated->buffer->SetCurrentPosition(0);
    soundBuffers.targetActivated->buffer->Play(0, 0, 0);
  }
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const play_screen_state& playState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnosticsData, controlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"bullet count: %I64u", playState.bullets.size());
  diagnosticsData.push_back(text);

  swprintf(text, L"initial ticks: %I64u", playState.levelTimer->initialTicks);
  diagnosticsData.push_back(text);

  swprintf(text, L"end ticks: %I64u", playState.levelTimer->endTicks);
  diagnosticsData.push_back(text);

  swprintf(text, L"current ticks: %I64u", playState.levelTimer->currentTicks);
  diagnosticsData.push_back(text);

  int64_t ticksRemaining = GetTicksRemaining(*playState.levelTimer);
  swprintf(text, L"remaining ticks: %I64u", ticksRemaining);
  diagnosticsData.push_back(text);
}
