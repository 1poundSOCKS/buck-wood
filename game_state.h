struct game_state
{
  game_state(int frameWidth, int frameHeight);

  double xPos;
  double yPos;
  double xVelocity;
  double yVelocity;
  double shipAngle;
};
