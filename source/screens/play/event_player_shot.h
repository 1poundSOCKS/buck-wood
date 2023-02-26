#pragma once

#include "player_ship.h"

struct event_player_shot
{
  event_player_shot(float x, float y, float angle) : x(x), y(y), angle(angle)
  {
  }

  auto Trigger() const -> void
  {
  // static const float bulletSpeed = 200.0f;
  // static const float bulletRange = 2000.0f;

  // auto& bullet = GetBullet(levelState.bullets);
  // bullet.startX = bullet.xPos = x;
  // bullet.startY = bullet.yPos = y;
  // bullet.angle = CalculateAngle(x, y, levelState.mouseX, levelState.mouseY);
  // bullet.yVelocity = -bulletSpeed * cos(DEGTORAD(bullet.angle));
  // bullet.xVelocity = bulletSpeed * sin(DEGTORAD(bullet.angle));
  // levelState.lastShotTimerValue = levelState.currentTimerCount;
  // levelState.playerShot = true;
  }

  float x;
  float y;
  float angle;
};
