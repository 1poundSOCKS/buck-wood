#include "input_state.h"

input_state::input_state()
{
  ZeroMemory(keyboardState, sizeof(keyboardState));
	ZeroMemory(&clientRect, sizeof(clientRect));
	ZeroMemory(&renderTargetSize, sizeof(renderTargetSize));
}

void input_state::RefreshKeyboard(const winrt::com_ptr<IDirectInputDevice8>& keyboard)
{
  HRESULT hr = keyboard->GetDeviceState(sizeof(keyboardState), keyboardState);
	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
	}
}

void input_state::UpdateMouse(LPARAM clientMouseX, LPARAM clientMouseY, const RECT& clientRect, 
															bool leftMouseButtonDown, bool rightMouseButtonDown, 
															const D2D1_SIZE_F& renderTargetSize)
{
  this->clientMouseX = clientMouseX;
  this->clientMouseY = clientMouseY;
  this->leftMouseButtonDown = leftMouseButtonDown;
  this->rightMouseButtonDown = rightMouseButtonDown;
	this->clientRect = clientRect;
	this->renderTargetSize = renderTargetSize;

  ratioMouseX = static_cast<float>(clientMouseX) / static_cast<float>(clientRect.right - clientRect.left);
  ratioMouseY = static_cast<float>(clientMouseY) / static_cast<float>(clientRect.bottom - clientRect.top);

  renderTargetMouseX = ratioMouseX * renderTargetSize.width;
  renderTargetMouseY = ratioMouseY * renderTargetSize.height;
}
