#include "game_state.h"
#include <list>

game_state::game_state()
: running(true), screen(title)
{
  cursor.size = 5.0;
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  return bullet->lifespanSeconds <= 0;
}

void game_state::Update(const control_state& controlState, float seconds)
{
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
    if( controlState.startGame ) screen = game_state::main;
    return;
  }

  if( controlState.left ) player.spin = -100.0f;
  else if( controlState.right ) player.spin = 100.0f;
  else player.spin = 0.0f;
  
  static const float forceOfGravity = 10.0f;
  static const float maxPlayerFallVelocity = 300.0f;
  static const float playerThrust = 20.0f;

  player.yVelocity += ( forceOfGravity );
  if( player.yVelocity > maxPlayerFallVelocity ) player.yVelocity = maxPlayerFallVelocity;
  if( controlState.shoot ) OnPlayerShoot();
  if( controlState.accelerate ) player.Accelerate(playerThrust);
  
  player.Update(seconds);

  for(const std::unique_ptr<bullet>& bullet : bullets)
  {
    bullet->Update(seconds);
  }
  
  bullets.remove_if(BulletHasExpired);

  if( PlayerIsOutOfBounds() )
  {
    player.xPos = currentLevel->width / 2;
    player.yPos = currentLevel->height / 2;
  }
}

bool game_state::PlayerIsOutOfBounds() const
{
  return currentLevel->OutOfBounds(player.xPos, player.yPos);  
}

void game_state::OnPlayerShoot()
{
  std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
  newBullet->gameObject.xPos = player.xPos;
  newBullet->gameObject.yPos = player.yPos;
  float angle = CalculateAngle(player.xPos, player.yPos, cursor.xPos, cursor.yPos);
  newBullet->gameObject.angle = angle;
  newBullet->gameObject.Accelerate(800.0f);
  bullets.push_front(std::move(newBullet));
}
