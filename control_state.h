struct control_state
{
  control_state() : quit(false), left(false), right(false), accelerate(false), shoot(false), mouseX(0), mouseY(0) {}
  
  bool quit;
  bool left;
  bool right;
  bool accelerate;
  bool shoot;
  int mouseX, mouseY;
};
