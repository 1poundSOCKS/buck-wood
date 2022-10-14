#include "game_state.h"
#include <list>

game_state::game_state(std::unique_ptr<game_level>& firstLevel)
: running(true), screen(title), playerState(alive), currentLevel(std::move(firstLevel))
{
  player = CreatePlayerShip();
  timer = std::make_unique<system_timer>();
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  float cx = bullet->xPos - bullet->startX;
  float cy = bullet->yPos - bullet->startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet->range || bullet->outsideLevel;
}

void UpdateGameState(game_state& gameState, const control_state& controlState)
{
  UpdateSystemTimer(*gameState.timer);
  float intervalTime = GetIntervalTimeInSeconds(*gameState.timer);

  static const float gameSpeedMultiplier = 2.0f;
  const float gameUpdateInterval = intervalTime * gameSpeedMultiplier;

  if( controlState.quitPress )
  {
    switch( gameState.screen )
    {
      case game_state::title:
        gameState.running = false;
        return;
      case game_state::main:
        gameState.screen = game_state::title;
        break;
    }
  }

  if( gameState.screen == game_state::title )
  {
    if( controlState.startGame ){
      gameState.screen = game_state::main;
      ResetGameState(gameState);
    }
    return;
  }

  if( gameState.playerState == game_state::alive )
  {
    UpdatePlayer(gameState, controlState, gameUpdateInterval);
    UpdateBullets(gameState, controlState, gameUpdateInterval);

    std::list<game_point> transformedPoints;
    TransformPlayerShip(*gameState.player, transformedPoints);

    if( PlayerIsOutOfBounds(gameState) || !PointsInside(transformedPoints, *gameState.currentLevel->boundary) )
    {
      gameState.playerState = game_state::dead;
      gameState.levelTimerStop = gameState.timer->totalTicks;
    }
    
    for( const auto& shape : gameState.currentLevel->objects)
    {
      if( PointInside(transformedPoints, *shape) )
      {
        gameState.playerState = game_state::dead;
        gameState.levelTimerStop = gameState.timer->totalTicks;
      }
    }
  }
}

bool PlayerIsOutOfBounds(const game_state& gameState)
{
  return gameState.currentLevel->OutOfBounds(gameState.player->xPos, gameState.player->yPos);  
}

std::unique_ptr<game_state> CreateInitialGameState()
{
  std::unique_ptr<game_state> gameState = std::make_unique<game_state>(CreateInitialGameLevel());
  ResetGameState(*gameState);
  return gameState;
}

void UpdatePlayer(game_state& gameState, const control_state& controlState, float gameUpdateInterval)
{
  static const float forceOfGravity = 1.0f;
  static const float playerThrust = 3.0f;
  static const float rotationSpeed = 100.0f;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState.accelerate )
  {
    forceX += playerThrust * sin(DEGTORAD(gameState.player->angle));
    forceY -= playerThrust * cos(DEGTORAD(gameState.player->angle));
  }
  
  float spin = 0.0f;
  if( controlState.left ) spin = -rotationSpeed;
  else if( controlState.right ) spin = rotationSpeed;
  
  gameState.player->xVelocity += forceX * gameUpdateInterval;
  gameState.player->yVelocity += forceY * gameUpdateInterval;
  gameState.player->xPos += gameState.player->xVelocity;
  gameState.player->yPos += gameState.player->yVelocity;
  gameState.player->angle += spin * gameUpdateInterval;
}

void UpdateBullets(game_state& gameState, const control_state& controlState, float gameUpdateInterval)
{
  if( controlState.shoot )
  {
    static const float bulletSpeed = 200.0f * gameUpdateInterval;
    static const float bulletRange = 2000.0f;

    float cursorX = controlState.gameMouseX;
    float cursorY = controlState.gameMouseY;

    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(gameState.player->xPos, gameState.player->yPos, bulletRange);
    float angle = CalculateAngle(gameState.player->xPos, gameState.player->yPos, cursorX, cursorY);
    newBullet->angle = angle;
    newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
    newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
    gameState.bullets.push_front(std::move(newBullet));
  }

  for(const auto& bullet : gameState.bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
    bullet->outsideLevel = !PointInside(bulletPoint, *gameState.currentLevel->boundary);

    for( const auto& shape : gameState.currentLevel->objects)
    {
      if( PointInside(bulletPoint, *shape) ) bullet->outsideLevel = true;
    }

    for( const auto& target: gameState.currentLevel->targets )
    {
    if( target->state == target::DEACTIVATED && PointInside(bulletPoint, target->shape) )
      {
        bullet->outsideLevel = true;
        target->state = target::ACTIVATED;
        gameState.levelTimerStop = gameState.timer->totalTicks;
      }
    }
  }
  
  gameState.bullets.remove_if(BulletHasExpired);
}

void ResetGameState(game_state& gameState)
{
  gameState.player->xPos = gameState.currentLevel->playerStartPosX;
  gameState.player->yPos = gameState.currentLevel->playerStartPosY;
  gameState.player->xVelocity = 0;
  gameState.player->yVelocity = 0;
  gameState.player->angle = 0;
  gameState.playerState = game_state::alive;
  gameState.bullets.clear();
  ResetGameLevel(*gameState.currentLevel);
  gameState.levelTimerStart = gameState.timer->totalTicks;
  gameState.levelTimerStop = 0;
}
