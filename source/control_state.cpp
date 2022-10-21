#include "control_state.h"

control_state::control_state()
: quit(false), quitPress(false), startGame(false), left(false), right(false), accelerate(false), shoot(false), 
  mouseX(0), mouseY(0), renderTargetMouseX(0), renderTargetMouseY(0)//, gameMouseX(0), gameMouseY(0)
{
}

std::unique_ptr<control_state> GetControlState(const d2d_app& app, const control_state& previousControlState)
{
  std::unique_ptr<control_state> cs = std::make_unique<control_state>();

  unsigned char keyboardState[256];
  HRESULT hr = app.keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) app.keyboard->Acquire();
	}

  if( SUCCEEDED(hr) )
  {
    if( keyboardState[DIK_ESCAPE] & 0x80 )
    {
      cs->quit = true;
      if( !previousControlState.quit ) cs->quitPress = true;
    }
    if( keyboardState[DIK_SPACE] & 0x80 ) cs->startGame = true;
    if( keyboardState[DIK_Z] & 0x80 ) cs->left = true;
    if( keyboardState[DIK_X] & 0x80 ) cs->right = true;
    if( keyboardState[DIK_SPACE] & 0x80 ) cs->accelerate = true;
    if( keyboardState[DIK_F1] & 0x80 ) cs->functionKey_1 = true;
  }

  POINT p;
  if( GetCursorPos(&p) )
  {
    POINT clientPos;
    if( ScreenToClient(app.wnd, &p) )
    {
      cs->mouseX = static_cast<float>(p.x) / static_cast<float>(app.windowWidth);
      cs->mouseY = static_cast<float>(p.y) / static_cast<float>(app.windowHeight);
      D2D1_SIZE_F renderTargetSize = app.d2d_rendertarget->GetSize();
      cs->renderTargetMouseX = cs->mouseX * renderTargetSize.width;
      cs->renderTargetMouseY = cs->mouseY * renderTargetSize.height;
    }
  }

#ifdef USE_DIRECTINPUT_MOUSE
  DIMOUSESTATE mouseState;
  hr = app.mouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<LPVOID>(&mouseState));
  if( FAILED(hr) )
  {
    if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) app.mouse->Acquire();
  }

  if( SUCCEEDED(hr) )
  {

    if( mouseState.rgbButtons[0] & 0x80 ) cs->shoot = true;
    if( mouseState.rgbButtons[1] & 0x80 ) cs->accelerate = true;
  }
#else
  if( app.mouseLButtonDown ) cs->shoot = true;
  if( app.mouseRButtonDown ) cs->accelerate = true;
#endif

  return cs;
}
