#include "play_screen.h"
#include "render.h"
#include "game_math.h"

const float gameSpeedMultiplier = 2.0f;

void RenderGamePaused(const d2d_frame& frame, const play_screen_state& screenState);
void RenderLevelComplete(const d2d_frame& frame, const play_screen_state& screenState);
void RenderGameComplete(const d2d_frame& frame, const play_screen_state& screenState);
void RenderPlayerDead(const d2d_frame& frame, const play_screen_state& screenState);

void OnPlay(play_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void OnLevelComplete(play_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdatePlayer(play_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdateBullets(play_screen_state& screenState, const control_state& controlState, const system_timer& timer);
bool LevelIsComplete(const play_screen_state& screenState);

const int play_screen_state::shotTimeNumerator;
const int play_screen_state::shotTimeDenominator;

play_screen_state::play_screen_state(const global_state& globalState, const system_timer& systemTimer) 
: systemTimer(systemTimer), 
  globalState(globalState),
  brushes(globalState.brushes),
  textFormats(globalState.textFormats)
{
  currentLevelDataIterator = globalState.gameLevelDataIndex->begin();
  const auto& levelData = *currentLevelDataIterator;
  currentLevel = std::make_unique<game_level>(*levelData);

  player = CreatePlayerShip();
  player->xPos = currentLevel->playerStartPosX;
  player->yPos = currentLevel->playerStartPosY;

  levelTimer = std::make_unique<stopwatch>(systemTimer, static_cast<int>(currentLevel->timeLimitInSeconds), 1);
  pauseTimer = std::make_unique<stopwatch>(systemTimer);
  shotTimer = std::make_unique<stopwatch>(systemTimer, shotTimeNumerator, shotTimeDenominator);
  shotTimer->paused = false;

  levelTimes.reserve(globalState.gameLevelDataIndex->size());

  state = play_screen_state::state_playing;  
}

play_screen_sounds::play_screen_sounds(const global_state& globalAssets)
: thrust(*globalAssets.soundBuffers.thrust),
  shoot(*globalAssets.soundBuffers.shoot),
  targetActivated(*globalAssets.soundBuffers.targetActivated)
{
}

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const play_screen_state& screenState)
{
  static const float renderScale = 1.0f;
  auto renderTargetSize = renderTarget->GetSize();
  return CreateGameLevelTransform(screenState.player->xPos, screenState.player->yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}

void RenderFrame(const d2d_frame& frame, const play_screen_state& screenState)
{
#ifdef EXPERIMENTAL
  float absVelocityX = fabsf(screenState.player->xVelocity);
  float absVelocityY = fabsf(screenState.player->yVelocity);
  float velocity = sqrt( absVelocityX * absVelocityX + absVelocityY * absVelocityY );
  const float renderScale = 1.5f - velocity / 500;
#else
  const float renderScale = 1.0f;
#endif

  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  auto& currentLevel = *screenState.currentLevel;

  RenderLevel(frame, currentLevel, screenState.brushes);
  RenderPlayer(frame, *screenState.player, screenState.brushes);

  for( const std::unique_ptr<bullet>& bullet : screenState.bullets )
  {
    RenderBullet(frame, *bullet, screenState.brushes);
  }

  switch( screenState.state )
  {
    case play_screen_state::state_paused:
      RenderGamePaused(frame, screenState);
      break;
    case play_screen_state::state_level_complete:
      RenderLevelComplete(frame, screenState);
      break;
    case play_screen_state::state_game_complete:
      RenderGameComplete(frame, screenState);
      break;
    case play_screen_state::state_player_dead:
      RenderPlayerDead(frame, screenState);
      break;
  }

  float levelTimeRemaining = GetTimeRemainingInSeconds(*screenState.levelTimer);
  RenderTimer(frame, levelTimeRemaining, screenState.textFormats, screenState.brushes);

  RenderMouseCursor(frame, screenState.mouseCursor, screenState.brushes);
}

void RenderGamePaused(const d2d_frame& frame, const play_screen_state& screenState)
{
  std::wstring text = L"PAUSED";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), screenState.textFormats.levelEndTextFormat.get(), rect, screenState.brushes.brushLevelEndText.get());
}

void RenderLevelComplete(const d2d_frame& frame, const play_screen_state& screenState)
{
  std::wstring text = L"LEVEL COMPLETE";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), screenState.textFormats.levelEndTextFormat.get(), rect, screenState.brushes.brushLevelEndText.get());
}

void RenderGameComplete(const d2d_frame& frame, const play_screen_state& screenState)
{
  std::wstring msg = L"";

  for( auto levelTime: screenState.levelTimes )
  {
    static wchar_t text[64];
    swprintf(text, L"%.2f\n", levelTime);
    msg += text;
  }

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(msg.c_str(),msg.length(), screenState.textFormats.levelEndTextFormat.get(), rect, screenState.brushes.brushLevelEndText.get());
}

void RenderPlayerDead(const d2d_frame& frame, const play_screen_state& screenState)
{
  std::wstring text = L"YOU LOSE";
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(text.c_str(),text.length(), screenState.textFormats.levelEndTextFormat.get(), rect, screenState.brushes.brushLevelEndText.get());
}

void UpdateScreenState(play_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.levelMouseX = controlState.worldMouseX;
  screenState.levelMouseY = controlState.worldMouseY;

  UpdateStopwatch(*screenState.levelTimer);
  UpdateStopwatch(*screenState.pauseTimer);
  UpdateStopwatch(*screenState.shotTimer);

  screenState.returnToMenu = screenState.playerShot = screenState.targetShot = false;

  if( screenState.state == play_screen_state::state_paused )
  {
    if( controlState.quitPress )
    {
      screenState.returnToMenu = true;
      return;
    }

    if( controlState.startGame )
    {
      screenState.state = play_screen_state::state_playing;
      screenState.levelTimer->paused = false;
      screenState.shotTimer->paused = false;
      return;
    }
  }
  
  if( GetTicksRemaining(*screenState.pauseTimer) > 0 ) return;

  if( screenState.state == play_screen_state::state_playing )
  {
    if( controlState.quitPress )
    {
      screenState.state = play_screen_state::state_paused;
      screenState.levelTimer->paused = true;
      screenState.shotTimer->paused = true;
      return;
    }

    screenState.levelTimer->paused = false;
    screenState.shotTimer->paused = false;
    OnPlay(screenState, controlState, timer);
    return;
  }

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
}

void OnPlay(play_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  int64_t ticksRemaining = GetTicksRemaining(*screenState.levelTimer);

  if( ticksRemaining == 0 )
  {
    screenState.state = play_screen_state::state_player_dead;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }
  
  UpdatePlayer(screenState, controlState, timer);
  UpdateBullets(screenState, controlState, timer);

  if( LevelIsComplete(screenState) )
  {
    screenState.state = play_screen_state::state_level_complete;
    screenState.levelTimer->paused = true;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }

  std::list<game_point> transformedPoints;
  TransformPlayerShip(*screenState.player, transformedPoints);

  if( !PointsInside(transformedPoints, *screenState.currentLevel->boundary) )
  {
    screenState.state = play_screen_state::state_player_dead;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }
  
  for( const auto& shape : screenState.currentLevel->objects)
  {
    if( PointInside(transformedPoints, *shape) )
    {
      screenState.state = play_screen_state::state_player_dead;
      ResetStopwatch(*screenState.pauseTimer, 3);
      screenState.pauseTimer->paused = false;
      return;
    }
  }
}

void OnLevelComplete(play_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  float levelTimeRemaining = GetTimeRemainingInSeconds(*screenState.levelTimer);
  screenState.levelTimes.push_back(levelTimeRemaining);

  screenState.currentLevelDataIterator++;
  
  if( screenState.currentLevelDataIterator == screenState.globalState.gameLevelDataIndex->end() )
  {
    screenState.state = play_screen_state::state_game_complete;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }

  const auto& nextLevelData = *screenState.currentLevelDataIterator;
  screenState.currentLevel = std::make_unique<game_level>(*nextLevelData);

  ResetStopwatch(*screenState.levelTimer, screenState.currentLevel->timeLimitInSeconds);
  ResetStopwatch(*screenState.shotTimer);

  screenState.player = CreatePlayerShip();
  screenState.player->xPos = screenState.currentLevel->playerStartPosX;
  screenState.player->yPos = screenState.currentLevel->playerStartPosY;

  screenState.bullets.clear();
  screenState.state = play_screen_state::state_playing;
}

bool LevelIsComplete(const play_screen_state& screenState)
{
  int activatedTargetCount = 0;
  for( const auto& target: screenState.currentLevel->targets )
  {
    if( target->state == target::ACTIVATED ) activatedTargetCount++;
  }

  return activatedTargetCount == screenState.currentLevel->targets.size();
}

void UpdatePlayer(play_screen_state& screenState, const control_state& controlState, const system_timer& timer)
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
    screenState.player->thrusterOn = true;
    forceX += playerThrust * sin(DEGTORAD(screenState.player->angle));
    forceY -= playerThrust * cos(DEGTORAD(screenState.player->angle));
  }
  else
  {
    screenState.player->thrusterOn = false;
  }
  
#ifdef USE_KEYBOARDFORSPIN
  float spin = 0.0f;
  if( controlState.left ) spin = -rotationSpeed;
  else if( controlState.right ) spin = rotationSpeed;
#endif
  
  screenState.player->xVelocity += forceX * gameUpdateInterval;
  screenState.player->yVelocity += forceY * gameUpdateInterval;
  screenState.player->xPos += screenState.player->xVelocity * gameUpdateInterval;
  screenState.player->yPos += screenState.player->yVelocity * gameUpdateInterval;
#ifdef USE_KEYBOARDFORSPIN
  screenState.player->angle += spin * gameUpdateInterval;
#else
  screenState.player->angle = CalculateAngle(screenState.player->xPos, screenState.player->yPos, screenState.levelMouseX, screenState.levelMouseY);
#endif
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  float cx = bullet->xPos - bullet->startX;
  float cy = bullet->yPos - bullet->startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet->range || bullet->outsideLevel;
}

void UpdateBullets(play_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  if( controlState.shoot && GetTicksRemaining(*screenState.shotTimer) == 0 )
  {
    static const float bulletSpeed = 200.0f * gameUpdateInterval;
    static const float bulletRange = 2000.0f;

    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(screenState.player->xPos, screenState.player->yPos, bulletRange);
    float angle = CalculateAngle(screenState.player->xPos, screenState.player->yPos, screenState.levelMouseX, screenState.levelMouseY);
    newBullet->angle = angle;
    newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
    newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
    screenState.bullets.push_front(std::move(newBullet));

    screenState.playerShot = true;
    ResetStopwatch(*screenState.shotTimer);
    screenState.shotTimer->paused = false;
  }

  for(const auto& bullet : screenState.bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
    bullet->outsideLevel = !PointInside(bulletPoint, *screenState.currentLevel->boundary);

    for( const auto& shape : screenState.currentLevel->objects)
    {
      if( PointInside(bulletPoint, *shape) ) bullet->outsideLevel = true;
    }

    for( const auto& target: screenState.currentLevel->targets )
    {
      if( PointInside(bulletPoint, target->shape) )
      {
        bullet->outsideLevel = true;

        if( target->state == target::DEACTIVATED )
        {
          target->state = target::ACTIVATED;
          screenState.targetShot = true;
        }
      }
    }
  }
  
  screenState.bullets.remove_if(BulletHasExpired);
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const play_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnosticsData, controlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"bullet count: %I64u", screenState.bullets.size());
  diagnosticsData.push_back(text);

  swprintf(text, L"initial ticks: %I64u", screenState.levelTimer->initialTicks);
  diagnosticsData.push_back(text);

  swprintf(text, L"end ticks: %I64u", screenState.levelTimer->endTicks);
  diagnosticsData.push_back(text);

  swprintf(text, L"current ticks: %I64u", screenState.levelTimer->currentTicks);
  diagnosticsData.push_back(text);

  int64_t ticksRemaining = GetTicksRemaining(*screenState.levelTimer);
  swprintf(text, L"remaining ticks: %I64u", ticksRemaining);
  diagnosticsData.push_back(text);
}

void UpdateSound(const play_screen_state& screenState, const play_screen_sounds& sounds)
{
  if( screenState.playerShot ) sounds.shoot.Play();
  if( screenState.targetShot ) sounds.targetActivated.Play();
  if( screenState.player->thrusterOn ) sounds.thrust.PlayOnLoop();
  else sounds.thrust.Stop();
}
