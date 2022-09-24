struct control_state
{
  control_state() : quit(false), left(false), right(false), accelerate(false), mouseX(0), mouseY(0) {}
  
  bool quit;
  bool left;
  bool right;
  bool accelerate;
  int mouseX, mouseY;
};
