#include "game_state.h"
#include <list>

game_state::game_state(std::unique_ptr<game_level>& firstLevel)
: running(true), speed(0.5f), screen(title), playerState(alive), currentLevel(std::move(firstLevel))
{
  player = CreatePlayerShip();
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
    TransformPlayerShip(*player, transformedPoints);

    if( PlayerIsOutOfBounds() || !PointsInside(transformedPoints, *currentLevel->boundary) ) playerState = game_state::dead;
    
    for( const auto& shape : currentLevel->objects)
    {
      if( PointInside(transformedPoints, *shape) ) playerState = game_state::dead;
    }
  }
}

void game_state::UpdatePlayer(const control_state& controlState, float seconds)
{
  static const float forceOfGravity = 1.0f;
  static const float playerThrust = 3.0f;
  static const float rotationSpeed = 400.0f;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState.accelerate )
  {
    forceX += playerThrust * sin(DEGTORAD(player->angle));
    forceY -= playerThrust * cos(DEGTORAD(player->angle));
  }
  
  float spin = 0.0f;
  if( controlState.left ) spin = -rotationSpeed;
  else if( controlState.right ) spin = rotationSpeed;
  
  player->xVelocity += forceX * seconds;
  player->yVelocity += forceY * seconds;
  player->xPos += player->xVelocity;
  player->yPos += player->yVelocity;
  player->angle += spin * seconds;
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
    float cursorX = controlState.mouseX * currentLevel->width;
    float cursorY = controlState.mouseY * currentLevel->height;

    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
    newBullet->xPos = player->xPos;
    newBullet->yPos = player->yPos;
    float angle = CalculateAngle(player->xPos, player->yPos, cursorX, cursorY);
    newBullet->angle = angle;
    newBullet->yVelocity = -1.0f * cos(DEGTORAD(angle));
    newBullet->xVelocity = 1.0f * sin(DEGTORAD(angle));
    bullets.push_front(std::move(newBullet));
  }

  for(const auto& bullet : bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;
    bullet->lifespanSeconds -= seconds;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
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
