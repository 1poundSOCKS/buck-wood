#include "play_screen.h"
#include "render.h"
#include "game_math.h"

const float gameSpeedMultiplier = 2.0f;

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const play_screen_state& screenState);
void RenderGamePaused(const d2d_frame& frame, const play_screen_state& screenState);
void RenderLevelComplete(const d2d_frame& frame, const play_screen_state& screenState);
void RenderGameComplete(const d2d_frame& frame, const play_screen_state& screenState);
void RenderPlayerDead(const d2d_frame& frame, const play_screen_state& screenState);

void OnPlay(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
void OnLevelComplete(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
void UpdatePlayer(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
void UpdateBullets(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
// bool LevelIsComplete(const play_screen_state& screenState);

const int play_screen_state::shotTimeNumerator;
const int play_screen_state::shotTimeDenominator;

player_ship::player_ship() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
{
}

bullet::bullet(float x, float y, float range) : startX(x), startY(y), xPos(x), yPos(y), range(range), xVelocity(0), yVelocity(0), angle(0), outsideLevel(false)
{
}

target_state::target_state(const game_point& position) : position(position)
{
  CreatePointsForTarget(position.x, position.y, 40, std::back_inserter(points));
}

play_screen_state::play_screen_state(const global_state& globalState, const system_timer& systemTimer) 
: systemTimer(systemTimer), 
  globalState(globalState),
  brushes(globalState.brushes),
  textFormats(globalState.textFormats),
  renderBrushes(globalState.renderBrushes)
{
  currentLevelDataIterator = globalState.gameLevelDataIndex->gameLevelData.begin();
  const auto& levelData = **currentLevelDataIterator;
  levelState = std::make_unique<level_state>(**currentLevelDataIterator);

  // player = std::make_unique<player_ship>();
  // player->xPos = levelData.playerStartPosX;
  // player->yPos = levelData.playerStartPosY;

  levelTimer = std::make_unique<stopwatch>(systemTimer, static_cast<int>(levelData.timeLimitInSeconds), 1);
  pauseTimer = std::make_unique<stopwatch>(systemTimer);
  shotTimer = std::make_unique<stopwatch>(systemTimer, shotTimeNumerator, shotTimeDenominator);
  shotTimer->paused = false;

  // std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [](const auto& target)
  // {
  //   return target_state(target);
  // });

  levelTimes.reserve(globalState.gameLevelDataIndex->gameLevelData.size());

  viewTransform = D2D1::Matrix3x2F::Identity();

  state = play_screen_state::state_playing;
}

play_screen_sounds::play_screen_sounds(const global_state& globalAssets)
: thrust(*globalAssets.soundBuffers.thrust),
  shoot(*globalAssets.soundBuffers.shoot),
  targetActivated(*globalAssets.soundBuffers.targetActivated)
{
}

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const play_screen_state& screenState)
{
  static const float renderScale = 1.0f;
  // return CreateGameLevelTransform(screenState.player->xPos, screenState.player->yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
  return CreateGameLevelTransform(screenState.levelState->player.xPos, screenState.levelState->player.yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState)
{
  controlState.pausePlay = baseControlState.escapeKeyPress;
  controlState.returnToMenu = baseControlState.escapeKeyPress;
  controlState.restartPlay = baseControlState.spacebarKeyPress;
  controlState.shoot = baseControlState.leftMouseButtonDown;
  controlState.thrust = baseControlState.rightMouseButtonDown;
  controlState.renderTargetMouseData = baseControlState.renderTargetMouseData;
}

void RenderFrame(const d2d_frame& frame, const play_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  D2D1_SIZE_F renderTargetSize = frame.renderTarget->GetSize();
  D2D1::Matrix3x2F viewTransform = CreateViewTransform(renderTargetSize, screenState);
  frame.renderTarget->SetTransform(viewTransform);

  std::vector<render_line> renderLines;
  CreateRenderLines(*screenState.levelState, std::back_inserter(renderLines));
  RenderLines(frame.renderTarget, screenState.renderBrushes, 2, renderLines.cbegin(), renderLines.cend());

  for( const std::unique_ptr<bullet>& bullet : screenState.levelState->bullets )
  {
    RenderBullet(frame.renderTarget, viewTransform, *bullet, screenState.brushes);
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
  RenderTimer(frame.renderTarget, levelTimeRemaining, screenState.textFormats, screenState.brushes);
}

void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const d2d_brushes& brushes)
{
  static const float bulletSize = 3.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  const D2D1::Matrix3x2F transform = translate * viewTransform;
  renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  renderTarget->FillRectangle(&rectangle, brushes.brush.get());
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

void UpdateScreenState(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer)
{
  screenState.renderTargetMouseX = controlState.renderTargetMouseData.x;
  screenState.renderTargetMouseY = controlState.renderTargetMouseData.y;

  D2D1::Matrix3x2F mouseTransform = CreateViewTransform(controlState.renderTargetMouseData.size, screenState);

  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = screenState.renderTargetMouseX;
    inPoint.y = screenState.renderTargetMouseY;
    auto outPoint = mouseTransform.TransformPoint(inPoint);
    screenState.levelMouseX = outPoint.x;
    screenState.levelMouseY = outPoint.y;
  }
  else
  {
    screenState.levelMouseX = 0;
    screenState.levelMouseY = 0;
  }

  UpdateStopwatch(*screenState.levelTimer);
  UpdateStopwatch(*screenState.pauseTimer);
  UpdateStopwatch(*screenState.shotTimer);

  screenState.returnToMenu = screenState.playerShot = screenState.targetShot = false;

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
      screenState.shotTimer->paused = false;
      return;
    }
  }
  
  if( GetTicksRemaining(*screenState.pauseTimer) > 0 ) return;

  if( screenState.state == play_screen_state::state_playing )
  {
    if( controlState.pausePlay )
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
  
  UpdatePlayer(screenState, controlState, timer);
  UpdateBullets(screenState, controlState, timer);

  if( LevelIsComplete(*screenState.levelState) )
  {
    screenState.state = play_screen_state::state_level_complete;
    screenState.levelTimer->paused = true;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }

  const level_state& levelState = *screenState.levelState;

  std::vector<game_point> player;
  CreatePointsForPlayer(levelState.player.xPos, levelState.player.yPos, levelState.player.angle, std::back_inserter(player));

  const auto& currentLevelData = levelState.levelData;

  std::vector<game_line> lines;
  CreateConnectedLines<game_point>(currentLevelData.boundaryPoints.cbegin(), currentLevelData.boundaryPoints.cend(), std::back_inserter(lines));
  if( !PointInside(player.cbegin(), player.cend(), lines) )
  {
    screenState.state = play_screen_state::state_player_dead;
    ResetStopwatch(*screenState.pauseTimer, 3);
    screenState.pauseTimer->paused = false;
    return;
  }
  
  for( const auto& object : currentLevelData.objects)
  {
    std::vector<game_line> lines;
    CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
    if( PointsInside(player.cbegin(), player.cend(), lines) )
    {
      screenState.state = play_screen_state::state_player_dead;
      ResetStopwatch(*screenState.pauseTimer, 3);
      screenState.pauseTimer->paused = false;
      return;
    }
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

  screenState.levelState = std::make_unique<level_state>(**screenState.currentLevelDataIterator);

  ResetStopwatch(*screenState.levelTimer, levelData.timeLimitInSeconds);
  ResetStopwatch(*screenState.shotTimer);

  // screenState.player = std::make_unique<player_ship>();
  screenState.levelState->player.xPos = levelData.playerStartPosX;
  screenState.levelState->player.yPos = levelData.playerStartPosY;

  screenState.levelState->bullets.clear();
  screenState.state = play_screen_state::state_playing;
}

// bool LevelIsComplete(const play_screen_state& screenState)
// {
//   int activatedTargetCount = 0;
//   for( const auto& target: screenState.targets )
//   {
//     if( target.activated ) activatedTargetCount++;
//   }

//   return activatedTargetCount == screenState.targets.size();
// }

void UpdatePlayer(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer)
{
  static const float forceOfGravity = 20.0f;
  static const float playerThrust = 100.0f;

  auto& levelState = *screenState.levelState;

  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState.thrust )
  {
    levelState.player.thrusterOn = true;
    forceX += playerThrust * sin(DEGTORAD(levelState.player.angle));
    forceY -= playerThrust * cos(DEGTORAD(levelState.player.angle));
  }
  else
  {
    levelState.player.thrusterOn = false;
  }
  
  levelState.player.xVelocity += forceX * gameUpdateInterval;
  levelState.player.yVelocity += forceY * gameUpdateInterval;
  levelState.player.xPos += levelState.player.xVelocity * gameUpdateInterval;
  levelState.player.yPos += levelState.player.yVelocity * gameUpdateInterval;
  levelState.player.angle = CalculateAngle(levelState.player.xPos, levelState.player.yPos, screenState.levelMouseX, screenState.levelMouseY);
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  float cx = bullet->xPos - bullet->startX;
  float cy = bullet->yPos - bullet->startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet->range || bullet->outsideLevel;
}

void UpdateBullets(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer)
{
  auto& levelState = *screenState.levelState;

  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  if( controlState.shoot && GetTicksRemaining(*screenState.shotTimer) == 0 )
  {
    static const float bulletSpeed = 200.0f * gameUpdateInterval;
    static const float bulletRange = 2000.0f;

    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(levelState.player.xPos, levelState.player.yPos, bulletRange);
    float angle = CalculateAngle(levelState.player.xPos, levelState.player.yPos, screenState.levelMouseX, screenState.levelMouseY);
    newBullet->angle = angle;
    newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
    newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
    levelState.bullets.push_front(std::move(newBullet));

    screenState.playerShot = true;
    ResetStopwatch(*screenState.shotTimer);
    screenState.shotTimer->paused = false;
  }

  const auto& levelData = levelState.levelData;

  for(const auto& bullet : levelState.bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
    std::vector<game_line> lines;
    CreateConnectedLines<game_point>(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(lines));
    bullet->outsideLevel = !PointInside(bulletPoint, lines);

    for( const auto& object : levelData.objects)
    {
      std::vector<game_line> lines;
      CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
      if( PointInside(bulletPoint, lines) ) bullet->outsideLevel = true;
    }

    for( auto& target: levelState.targets )
    {
      std::vector<game_line> lines;
      CreateConnectedLines<game_point>(target.points.cbegin(), target.points.cend(), std::back_inserter(lines));

      if( PointInside(bulletPoint, lines) )
      {
        bullet->outsideLevel = true;

        if( !target.activated )
        {
          target.activated = true;
          screenState.targetShot = true;
        }
      }
    }
  }
  
  levelState.bullets.remove_if(BulletHasExpired);
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const play_screen_state& screenState, const play_screen_control_state& controlState)
{
  auto& levelState = *screenState.levelState;

  static wchar_t text[64];

  swprintf(text, L"world mouse X: %.1f", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"world mouse Y: %.1f", screenState.levelMouseY);
  diagnosticsData.push_back(text);

  swprintf(text, L"bullet count: %I64u", levelState.bullets.size());
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
  auto& levelState = *screenState.levelState;

  if( screenState.playerShot ) sounds.shoot.Play();
  if( screenState.targetShot ) sounds.targetActivated.Play();
  if( levelState.player.thrusterOn && screenState.state == play_screen_state::state_playing ) sounds.thrust.PlayOnLoop();
  else sounds.thrust.Stop();
}
