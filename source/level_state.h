#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "control_state.h"

struct player_ship
{
  player_ship();

  enum player_state { state_alive, state_dead };

  player_state state = state_alive;

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  bool thrusterOn = false;

  std::vector<game_point> points;
  std::vector<game_point> transformedPoints;
  std::vector<game_point> thrusterPoints;
};

struct bullet
{
  bullet(float x, float y, float range);

  float startX, startY;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float range;
  bool outsideLevel;
};

struct target_state
{
  target_state(const game_point& position);

  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
  std::vector<game_line> shape;
};

struct level_control_state
{
  bool thrust = false, shoot = false;
  render_target_mouse_data renderTargetMouseData;
};

struct level_state
{
  level_state(const game_level_data& levelData, const system_timer& systemTimer);

  const game_level_data& levelData;
  const system_timer& systemTimer;
  player_ship player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::vector<target_state> targets;
  float mouseX = 0, mouseY = 0;
  stopwatch shotTimer;
  bool playerShot = false, targetShot = false;
  D2D1::Matrix3x2F viewTransform;

  using shape = std::vector<game_line>;

  shape boundaryLines;
  std::vector<shape> objectShapes;
  std::vector<shape> targetShapes;
};

void RefreshControlState(level_control_state& controlState, const control_state& baseControlState);
void UpdateState(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
bool LevelIsComplete(const level_state& levelState);
void RenderFrame(const d2d_frame& frame, const level_state& levelState, const render_brushes& brushes);
void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const render_brushes& brushes);
void CreateRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines);

#endif
