#include <d2d1.h>
#include <winrt/base.h>

struct game_object
{
  game_object() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0) {}
  D2D1_RECT_F GetRectangleForRender() const;
  void UpdatePosition();

  double xPos, yPos;
  double xVelocity, yVelocity;
  double angle;
  D2D1_RECT_F rectangle = D2D1::RectF(-10, -10, 10, 10);
};

struct game_state
{
  game_state(int frameWidth, int frameHeight);

  bool started;
  game_object player;
};
