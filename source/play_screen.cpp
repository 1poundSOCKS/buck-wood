#include "play_screen.h"
#include "render.h"
#include "game_math.h"

const float play_state::gameSpeedMultiplier = 2.0f;

D2D1::Matrix3x2F CreateLevelTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const play_state& playState);
void UpdatePlayer(play_state& playState, const control_state& controlState, const system_timer& timer);
void UpdateBullets(play_state& playState, const control_state& controlState, const system_timer& timer);
bool PlayerIsOutOfBounds(const play_state& playState);
play_state::LEVEL_STATE GetLevelState(const play_state& playState);

play_state::play_state(const system_timer& timer, const game_data_ptr& gameDataPtr) : gameData(gameDataPtr)
{
  state = play_state::state_playing;
  
  totalTicks = timer.totalTicks;
  ticksPerSecond = timer.ticksPerSecond;
  levelTimerStart = timer.totalTicks;
  lastShotTicks = timer.totalTicks;

  currentLevelDataIterator = gameData->begin();
  const game_level_data_ptr& levelData = *currentLevelDataIterator;
  currentLevel = std::make_shared<game_level>(*levelData);

  player = CreatePlayerShip();
  player->xPos = currentLevel->playerStartPosX;
  player->yPos = currentLevel->playerStartPosY;
}

void RenderFrame(const d2d_frame& frame, play_state& playState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  auto& currentLevel = *playState.currentLevel;

  auto levelTransform = CreateLevelTransform(frame.renderTarget, playState);

  RenderLevel(currentLevel, frame, levelTransform);

  RenderPlayer(*playState.player, frame, levelTransform);

  for( const std::unique_ptr<bullet>& bullet : playState.bullets )
  {
    RenderBullet(*bullet, frame, levelTransform);
  }

  if( playState.state == play_state::state_levelend )
  {
    std::wstring text = L"LEVEL COMPLETE";
    D2D_SIZE_F size = frame.renderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
    frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
  }
  else if( playState.state == play_state::state_complete )
  {
    std::wstring text = L"F*CK YEAH";
    D2D_SIZE_F size = frame.renderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
    frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
  }
  else if( playState.playerState == play_state::player_dead )
  {
    std::wstring text = L"YOU LOSE";
    D2D_SIZE_F size = frame.renderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
    frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
  }

  RenderTimer(frame, playState.levelTimeRemaining);

  if( levelTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = frame.renderTargetMouseX;
    inPoint.y = frame.renderTargetMouseY;
    D2D1_POINT_2F outPoint = levelTransform.TransformPoint(inPoint);
    playState.levelMouseX = outPoint.x;
    playState.levelMouseY = outPoint.y;
  }
}

D2D1::Matrix3x2F CreateLevelTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const play_state& playState)
{
  float playerPosX = playState.player->xPos;
  float playerPosY = playState.player->yPos;

  D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

  float shiftX = renderTargetSize.width / 2 - playerPosX;
  float shiftY = renderTargetSize.height / 2 - playerPosY;
  
  D2D1::Matrix3x2F matrixShift = D2D1::Matrix3x2F::Translation(shiftX, shiftY);
  
  return matrixShift;
}

void UpdateState(play_state& playState, const control_state& controlState, const system_timer& timer)
{
  playState.totalTicks = timer.totalTicks;

  playState.playerShot = playState.targetShot = false;

  if( controlState.quitPress )
  {
    playState.returnToMenu = true;
    return;
  }

  if( playState.state == play_state::state_complete )
  {
    float timeInSeconds = GetElapsedTimeInSeconds(playState.levelEndTickCount, timer.totalTicks, timer.ticksPerSecond);
    if( timeInSeconds < 5 ) return;
    playState.returnToMenu = true;
    return;
  }

  if( playState.playerState == play_state::player_dead ) return;

  if( playState.state == play_state::state_playing && playState.levelState == play_state::level_complete )
  {
    playState.state = play_state::state_levelend;
    playState.levelEndTickCount = timer.totalTicks;
    return;
  }

  if( playState.state == play_state::state_levelend )
  {
    float timeInSeconds = GetElapsedTimeInSeconds(playState.levelEndTickCount, timer.totalTicks, timer.ticksPerSecond);
    
    if( timeInSeconds < 3 ) return;

    playState.currentLevelDataIterator++;
    
    if( playState.currentLevelDataIterator != playState.gameData->end() )
    {
      playState.levelTimerStart = timer.totalTicks;
      playState.levelTimerStop = 0;
      playState.lastShotTicks = timer.totalTicks;
      game_level_data_ptr& nextLevelData = *playState.currentLevelDataIterator;
      playState.currentLevel = std::make_shared<game_level>(*nextLevelData);
      playState.levelState = play_state::level_incomplete;

      playState.player = CreatePlayerShip();
      playState.player->xPos = playState.currentLevel->playerStartPosX;
      playState.player->yPos = playState.currentLevel->playerStartPosY;

      playState.bullets.clear();
      playState.state = play_state::state_playing;
    }
    else
    {
      playState.state = play_state::state_complete;
    }
    return;
  }

  if( playState.state != play_state::state_playing ) return;

  float levelTimerInSeconds = playState.levelTimerStop == 0 ? 
    GetElapsedTimeInSeconds(playState.levelTimerStart, playState.totalTicks, playState.ticksPerSecond) :
    GetElapsedTimeInSeconds(playState.levelTimerStart, playState.levelTimerStop, playState.ticksPerSecond);

  playState.levelTimeRemaining = max(0, playState.currentLevel->timeLimitInSeconds - levelTimerInSeconds);

  if( playState.levelTimeRemaining == 0 )
  {
    playState.playerState = play_state::player_dead;
    return;
  }
  
  UpdatePlayer(playState, controlState, timer);
  UpdateBullets(playState, controlState, timer);

  playState.levelState = GetLevelState(playState);
  if( playState.levelState == play_state::level_complete ) playState.levelTimerStop = timer.totalTicks;

  std::list<game_point> transformedPoints;
  TransformPlayerShip(*playState.player, transformedPoints);

  if( PlayerIsOutOfBounds(playState) || !PointsInside(transformedPoints, *playState.currentLevel->boundary) )
  {
    playState.playerState = play_state::player_dead;
    playState.levelTimerStop = timer.totalTicks;
  }
  
  for( const auto& shape : playState.currentLevel->objects)
  {
    if( PointInside(transformedPoints, *shape) )
    {
      playState.playerState = play_state::player_dead;
      playState.levelTimerStop = timer.totalTicks;
    }
  }
}

play_state::LEVEL_STATE GetLevelState(const play_state& playState)
{
  int activatedTargetCount = 0;
  for( const auto& target: playState.currentLevel->targets )
  {
    if( target->state == target::ACTIVATED ) activatedTargetCount++;
  }

  if( activatedTargetCount == playState.currentLevel->targets.size() ) return play_state::level_complete;
  
  return play_state::level_incomplete;
}

bool PlayerIsOutOfBounds(const play_state& playState)
{
  return OutOfGameLevelBoundary(*playState.currentLevel, playState.player->xPos, playState.player->yPos);
}

void UpdatePlayer(play_state& playState, const control_state& controlState, const system_timer& timer)
{
  static const float forceOfGravity = 20.0f;
  static const float playerThrust = 80.0f;
  static const float rotationSpeed = 150.0f;

  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * play_state::gameSpeedMultiplier;

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
  
  float spin = 0.0f;
  if( controlState.left ) spin = -rotationSpeed;
  else if( controlState.right ) spin = rotationSpeed;
  
  playState.player->xVelocity += forceX * gameUpdateInterval;
  playState.player->yVelocity += forceY * gameUpdateInterval;
  playState.player->xPos += playState.player->xVelocity * gameUpdateInterval;
  playState.player->yPos += playState.player->yVelocity * gameUpdateInterval;
  playState.player->angle += spin * gameUpdateInterval;
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  float cx = bullet->xPos - bullet->startX;
  float cy = bullet->yPos - bullet->startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet->range || bullet->outsideLevel;
}

void UpdateBullets(play_state& playState, const control_state& controlState, const system_timer& timer)
{
  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * play_state::gameSpeedMultiplier;

  if( controlState.shoot )
  {
    float ticksSinceLastShot = timer.totalTicks - playState.lastShotTicks;

    static const float shotPerSecond = 60.0f;
    static const float ticksPerShot = timer.ticksPerSecond / shotPerSecond;

    if( ticksSinceLastShot >= ticksPerShot )
    {
      static const float bulletSpeed = 200.0f * gameUpdateInterval;
      static const float bulletRange = 2000.0f;

      float cursorX = playState.levelMouseX;
      float cursorY = playState.levelMouseY;

      std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(playState.player->xPos, playState.player->yPos, bulletRange);
      float angle = CalculateAngle(playState.player->xPos, playState.player->yPos, cursorX, cursorY);
      newBullet->angle = angle;
      newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
      newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
      playState.bullets.push_front(std::move(newBullet));

      playState.playerShot = true;
      playState.lastShotTicks = timer.totalTicks;
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

void UpdateSound(const sound_buffers& soundBuffers, const play_state& playState)
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
          playState.playerState == play_state::player_dead ||
          playState.levelState == play_state::level_complete )
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
