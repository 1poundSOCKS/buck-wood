#ifndef _d2d_app_
#define _d2d_app_

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <dxgi.h>
#include <d3d11.h>
#include <dinput.h>
#include <mmsystem.h>
#include <dsound.h>
#include <memory>
#include <winrt/base.h>
#include "system_timer.h"
#include "control_state.h"
#include "perf_data.h"
#include "d2d_brushes.h"
#include "dwrite_text_formats.h"

struct d2d_app
{
  d2d_app(HINSTANCE inst, int cmdShow, int fps);
  ~d2d_app();

  bool terminating;
  HINSTANCE inst;
  int cmdShow;
  HWND wnd;
  UINT windowWidth, windowHeight;
  std::unique_ptr<system_timer> timer;
  std::unique_ptr<control_state> controlState;
  std::unique_ptr<control_state> previousControlState;
  std::unique_ptr<perf_data> perfData;
  winrt::com_ptr<ID3D11Device> d3d_device;
  winrt::com_ptr<IDXGISwapChain> dxgi_swapChain;
  winrt::com_ptr<IDXGIDevice> dxgi_device;
  winrt::com_ptr<ID2D1Factory> d2d_factory;
  winrt::com_ptr<ID2D1RenderTarget> d2d_rendertarget;
  winrt::com_ptr<IDirectInput8> directInput;
  winrt::com_ptr<IDirectInputDevice8> keyboard;
#ifdef USE_DIRECTINPUT_MOUSE
  winrt::com_ptr<IDirectInputDevice8> mouse;
#endif
  winrt::com_ptr<IDirectSound8> directSound;
  winrt::com_ptr<IDirectSoundBuffer> primarySoundBuffer;
  d2d_brushes_ptr brushes;
  dwrite_text_formats_ptr textFormats;
  float mouseX, mouseY;
  bool mouseLButtonDown = false, mouseRButtonDown = false;
};

std::unique_ptr<control_state> GetControlState(const d2d_app& app, const control_state& previousControlState);
winrt::com_ptr<IDirectSoundBuffer> CreatePrimarySoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound);

#endif
