struct game_state
{
  game_state(int frameWidth, int frameHeight);

  bool started;
  double xPos;
  double yPos;
  double xVelocity;
  double yVelocity;
  double shipAngle;
};
