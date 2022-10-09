#include "game_state.h"
#include <list>

game_state::game_state(std::unique_ptr<game_level>& firstLevel)
: running(true), speed(0.5f), screen(title), playerState(alive), currentLevel(std::move(firstLevel))
{
  cursor = CreateCursorObject();
  cursor->size = 5.0;
  player = CreatePlayerObject();
  ResetPlayer();
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  return bullet->lifespanSeconds <= 0 || bullet->outsideLevel;
}

void game_state::Update(const control_state& controlState, float seconds)
{
  seconds *= speed;

  if( controlState.quitPress )
  {
    switch( screen )
    {
      case game_state::title:
        running = false;
        return;
      case game_state::main:
        screen = game_state::title;
        break;
    }
  }

  if( screen == game_state::title )
  {
    if( controlState.startGame )
    {
       screen = game_state::main;
      playerState = game_state::alive;
      ResetPlayer();
      bullets.clear();
    }
    return;
  }

  if( playerState == game_state::alive )
  {
    UpdatePlayer(controlState, seconds);
    UpdateBullets(controlState, seconds);

    std::list<game_point> transformedPoints;
    CalculateTransformedPoints(*player, transformedPoints);

    if( PlayerIsOutOfBounds() || !PointsInside(transformedPoints, *currentLevel->boundary) ) playerState = game_state::dead;
    
    for( const auto& shape : currentLevel->objects)
    {
      if( PointInside(transformedPoints, *shape) ) playerState = game_state::dead;
    }
  }
}

void game_state::UpdatePlayer(const control_state& controlState, float seconds)
{
  player->spin = 0.0f;
  if( controlState.left ) player->spin = -400.0f;
  else if( controlState.right ) player->spin = 400.0f;
  
  static const float forceOfGravity = 1.0f;
  static const float maxPlayerFallVelocity = 300.0f;
  static const float playerThrust = 3.0f;

  player->forceY = forceOfGravity;
  player->forceX = 0.0f;
  if( controlState.accelerate )
  {
    player->forceY -= playerThrust * cos(DEGTORAD(player->angle));
    player->forceX += playerThrust * sin(DEGTORAD(player->angle));
  }
  
  player->xVelocity += player->forceX * seconds;
  player->yVelocity += player->forceY * seconds;
  player->xPos += player->xVelocity;
  player->yPos += player->yVelocity;
  player->angle += player->spin * seconds;
}

void game_state::ResetPlayer()
{
  player->xPos = currentLevel->playerStartPosX;
  player->yPos = currentLevel->playerStartPosY;
  player->xVelocity = 0;
  player->yVelocity = 0;
  player->angle = 0;
}

void game_state::UpdateBullets(const control_state& controlState, float seconds)
{
  if( controlState.shoot )
  {
    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
    newBullet->gameObject.xPos = player->xPos;
    newBullet->gameObject.yPos = player->yPos;
    float angle = CalculateAngle(player->xPos, player->yPos, cursor->xPos, cursor->yPos);
    newBullet->gameObject.angle = angle;
    newBullet->gameObject.yVelocity = -1.0f * cos(DEGTORAD(angle));
    newBullet->gameObject.xVelocity = 1.0f * sin(DEGTORAD(angle));
    bullets.push_front(std::move(newBullet));
  }

  for(const auto& bullet : bullets)
  {
    bullet->gameObject.xPos += bullet->gameObject.xVelocity;
    bullet->gameObject.yPos += bullet->gameObject.yVelocity;
    bullet->lifespanSeconds -= seconds;

    const game_point bulletPoint(bullet->gameObject.xPos, bullet->gameObject.yPos);
    bullet->outsideLevel = !PointInside(bulletPoint, *currentLevel->boundary);

    for( const auto& shape : currentLevel->objects)
    {
      if( PointInside(bulletPoint, *shape) ) bullet->outsideLevel = true;
    }
  }
  
  bullets.remove_if(BulletHasExpired);
}

bool game_state::PlayerIsOutOfBounds() const
{
  return currentLevel->OutOfBounds(player->xPos, player->yPos);  
}

std::unique_ptr<game_state> CreateInitialGameState()
{
  std::unique_ptr<game_state> gameState = std::make_unique<game_state>(CreateInitialGameLevel());
  gameState->player->xPos = gameState->currentLevel->width / 2.0f;
  gameState->player->yPos = gameState->currentLevel->height / 2.0f;
  return gameState;
}
