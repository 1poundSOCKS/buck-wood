#include "play_state.h"

const float play_state::gameSpeedMultiplier = 2.0f;

play_state::play_state(const system_timer& timer, const game_level_ptr& firstLevel) : timer(timer), playerState(player_alive), currentLevel(std::move(firstLevel))
{
  player = CreatePlayerShip();
  player->xPos = currentLevel->playerStartPosX;
  player->yPos = currentLevel->playerStartPosY;
  levelTimerStart = timer.totalTicks;
  lastShotTicks = timer.totalTicks;
}

game_events_ptr UpdatePlayState(play_state& playState, const control_state& controlState)
{
  game_events_ptr events = std::make_shared<game_events>();

  if( playState.playerState == play_state::player_dead || playState.levelState == play_state::level_complete ) return events;

  UpdatePlayer(playState, controlState);
  UpdateBullets(playState, controlState, *events);

  playState.levelState = GetLevelState(playState);
  if( playState.levelState == play_state::level_complete ) playState.levelTimerStop = playState.timer.totalTicks;

  std::list<game_point> transformedPoints;
  TransformPlayerShip(*playState.player, transformedPoints);

  if( PlayerIsOutOfBounds(playState) || !PointsInside(transformedPoints, *playState.currentLevel->boundary) )
  {
    playState.playerState = play_state::player_dead;
    playState.levelTimerStop = playState.timer.totalTicks;
  }
  
  for( const auto& shape : playState.currentLevel->objects)
  {
    if( PointInside(transformedPoints, *shape) )
    {
      playState.playerState = play_state::player_dead;
      playState.levelTimerStop = playState.timer.totalTicks;
    }
  }

  return events;
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

void UpdatePlayer(play_state& playState, const control_state& controlState)
{
  static const float forceOfGravity = 20.0f;
  static const float playerThrust = 80.0f;
  static const float rotationSpeed = 150.0f;

  float gameUpdateInterval = GetIntervalTimeInSeconds(playState.timer) * play_state::gameSpeedMultiplier;

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

void UpdateBullets(play_state& playState, const control_state& controlState, game_events& events)
{
  float gameUpdateInterval = GetIntervalTimeInSeconds(playState.timer) * play_state::gameSpeedMultiplier;

  if( controlState.shoot )
  {
    float ticksSinceLastShot = playState.timer.totalTicks - playState.lastShotTicks;

    static const float shotPerSecond = 60.0f;
    static const float ticksPerShot = playState.timer.ticksPerSecond / shotPerSecond;

    if( ticksSinceLastShot >= ticksPerShot )
    {
      static const float bulletSpeed = 200.0f * gameUpdateInterval;
      static const float bulletRange = 2000.0f;

      float cursorX = controlState.gameMouseX;
      float cursorY = controlState.gameMouseY;

      std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(playState.player->xPos, playState.player->yPos, bulletRange);
      float angle = CalculateAngle(playState.player->xPos, playState.player->yPos, cursorX, cursorY);
      newBullet->angle = angle;
      newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
      newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
      playState.bullets.push_front(std::move(newBullet));

      events.playerShot = true;
      playState.lastShotTicks = playState.timer.totalTicks;
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
          events.targetShot = true;
        }
      }
    }
  }
  
  playState.bullets.remove_if(BulletHasExpired);
}
