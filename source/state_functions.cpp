#include "state_functions.h"

const float play_state::gameSpeedMultiplier = 2.0f;

screen_type UpdateState(game_state& gameState, const control_state& controlState, const system_timer& timer)
{
  if( gameState.starting )
  {
    gameState.starting = false;
    return screen_menu;
  }

  if( controlState.quitPress ) return screen_none;

  if( controlState.startGame ) return screen_play;

  return screen_menu;
}

screen_type UpdateState(play_state& playState, const control_state& controlState, const system_timer& timer)
{
  playState.totalTicks = timer.totalTicks;

  playState.playerShot = playState.targetShot = false;

  if( controlState.quitPress ) return screen_menu;

  if( playState.state == play_state::state_complete )
  {
    float timeInSeconds = GetElapsedTimeInSeconds(playState.levelEndTickCount, timer.totalTicks, timer.ticksPerSecond);
    if( timeInSeconds < 6 ) return screen_play;
    else return screen_menu;
  }

  if( playState.playerState == play_state::player_dead )
  {
    return screen_play;
  }

  if( playState.state == play_state::state_playing && playState.levelState == play_state::level_complete )
  {
    playState.state = play_state::state_levelend;
    playState.levelEndTickCount = timer.totalTicks;
    return screen_play;
  }

  if( playState.state == play_state::state_levelend )
  {
    float timeInSeconds = GetElapsedTimeInSeconds(playState.levelEndTickCount, timer.totalTicks, timer.ticksPerSecond);
    
    if( timeInSeconds < 3 ) return screen_play;

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
    return screen_play;
  }

  if( playState.state == play_state::state_playing )
  {
    float levelTimerInSeconds = playState.levelTimerStop == 0 ? 
      GetElapsedTimeInSeconds(playState.levelTimerStart, playState.totalTicks, playState.ticksPerSecond) :
      GetElapsedTimeInSeconds(playState.levelTimerStart, playState.levelTimerStop, playState.ticksPerSecond);

    playState.levelTimeRemaining = max(0, playState.currentLevel->timeLimitInSeconds - levelTimerInSeconds);

    if( playState.levelTimeRemaining == 0 )
    {
      playState.playerState = play_state::player_dead;
      return screen_play;
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
    return screen_play;
  }

  return screen_play;
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
