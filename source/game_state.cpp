#include "game_state.h"
#include <list>

game_state::game_state(std::unique_ptr<game_level>& firstLevel)
: running(true), speed(0.5f), screen(title), playerState(alive), currentLevel(std::move(firstLevel))
{
  player = CreatePlayerShip();
  Reset(*this);
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
    if( controlState.startGame ){
      screen = game_state::main;
      playerState = game_state::alive;
      Reset(*this);
    }
    return;
  }

  if( playerState == game_state::alive )
  {
    ::UpdatePlayer(*player, controlState, seconds);
    ::UpdateBullets(*this, controlState, seconds);

    std::list<game_point> transformedPoints;
    TransformPlayerShip(*player, transformedPoints);

    if( PlayerIsOutOfBounds() || !PointsInside(transformedPoints, *currentLevel->boundary) ) playerState = game_state::dead;
    
    for( const auto& shape : currentLevel->objects)
    {
      if( PointInside(transformedPoints, *shape) ) playerState = game_state::dead;
    }
  }
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

void UpdatePlayer(player_ship& player, const control_state& controlState, float seconds)
{
  static const float forceOfGravity = 1.0f;
  static const float playerThrust = 3.0f;
  static const float rotationSpeed = 400.0f;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState.accelerate )
  {
    forceX += playerThrust * sin(DEGTORAD(player.angle));
    forceY -= playerThrust * cos(DEGTORAD(player.angle));
  }
  
  float spin = 0.0f;
  if( controlState.left ) spin = -rotationSpeed;
  else if( controlState.right ) spin = rotationSpeed;
  
  player.xVelocity += forceX * seconds;
  player.yVelocity += forceY * seconds;
  player.xPos += player.xVelocity;
  player.yPos += player.yVelocity;
  player.angle += spin * seconds;
}

void UpdateBullets(game_state& gameState, const control_state& controlState, float seconds)
{
  if( controlState.shoot )
  {
    float cursorX = controlState.mouseX * gameState.currentLevel->width;
    float cursorY = controlState.mouseY * gameState.currentLevel->height;

    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
    newBullet->xPos = gameState.player->xPos;
    newBullet->yPos = gameState.player->yPos;
    float angle = CalculateAngle(gameState.player->xPos, gameState.player->yPos, cursorX, cursorY);
    newBullet->angle = angle;
    newBullet->yVelocity = -1.0f * cos(DEGTORAD(angle));
    newBullet->xVelocity = 1.0f * sin(DEGTORAD(angle));
    gameState.bullets.push_front(std::move(newBullet));
  }

  for(const auto& bullet : gameState.bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;
    bullet->lifespanSeconds -= seconds;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
    bullet->outsideLevel = !PointInside(bulletPoint, *gameState.currentLevel->boundary);

    for( const auto& shape : gameState.currentLevel->objects)
    {
      if( PointInside(bulletPoint, *shape) ) bullet->outsideLevel = true;
    }
  }
  
  gameState.bullets.remove_if(BulletHasExpired);
}

void Reset(game_state& gameState)
{
  gameState.player->xPos = gameState.currentLevel->playerStartPosX;
  gameState.player->yPos = gameState.currentLevel->playerStartPosY;
  gameState.player->xVelocity = 0;
  gameState.player->yVelocity = 0;
  gameState.player->angle = 0;
  
  gameState.bullets.clear();
}
