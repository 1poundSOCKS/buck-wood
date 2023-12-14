#include "pch.h"
#include "directx_functions.h"
#include "log.h"

auto CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> d3d_device_and_swap_chain
{
  d3d_device_and_swap_chain deviceAndSwapChain;
  auto& [d3dDevice, swapChain] = deviceAndSwapChain;
  
  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
    featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc, swapChain.put(), d3dDevice.put(), NULL, NULL);

  if( SUCCEEDED(hr) )
  {
    log::file() << "create d3d device & swap chain - SUCCESS\n";
  }
  else
  {
    log::file() << "create d3d device & swap chain - FAILED\n";
  }

  return deviceAndSwapChain;
}

auto GetDXGIDevice(ID3D11Device* d3dDevice) -> winrt::com_ptr<IDXGIDevice>
{
  winrt::com_ptr<IDXGIDevice> dxgiDevice;
  HRESULT hr = d3dDevice->QueryInterface(dxgiDevice.put());

  if( SUCCEEDED(hr) )
  {
    log::file() << "get dxgi device - SUCCESS\n";
  }
  else
  {
    log::file() << "get dxgi device - FAILED\n";
  }

  return dxgiDevice;
}

auto CreateD2DFactory() -> winrt::com_ptr<ID2D1Factory>
{
  winrt::com_ptr<ID2D1Factory> d2dFactory;
	
  HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,d2dFactory.put());

  if( SUCCEEDED(hr) )
  {
    log::file() << "create d2d factory - SUCCESS\n";
  }
  else
  {
    log::file() << "create d2d factory - FAILED\n";
  }
  
  return d2dFactory;
}

auto CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1RenderTarget>
{
  winrt::com_ptr<ID2D1RenderTarget> renderTarget;

  winrt::com_ptr<IDXGISurface> dxgi_surface;
  HRESULT hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgi_surface.put_void());

  if( SUCCEEDED(hr) )
  {
    log::file() << "get dxgi surface - SUCCESS\n";
  }
  else
  {
    log::file() << "get dxgi surface - FAILED\n";
  }

  D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE));
  
  hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgi_surface.get(), props, renderTarget.put());

  if( SUCCEEDED(hr) )
  {
    log::file() << "create render target on dxgi surface - SUCCESS\n";
  }
  else
  {
    log::file() << "create render target on dxgi surface - FAILED\n";
  }

  return renderTarget;
}

auto CreateDWriteFactory() -> winrt::com_ptr<IDWriteFactory>
{
  winrt::com_ptr<IDWriteFactory> dwriteFactory;
  HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(dwriteFactory),reinterpret_cast<IUnknown**>(dwriteFactory.put()));

  if( SUCCEEDED(hr) )
  {
    log::file() << "create dwrite factory - SUCCESS\n";
  }
  else
  {
    log::file() << "create dwrite factory - FAILED\n";
  }

  return dwriteFactory;
}

auto CreatePathGeometry(ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1PathGeometry>
{
  winrt::com_ptr<ID2D1PathGeometry> pathGeometry;

  HRESULT hr = d2dFactory->CreatePathGeometry(pathGeometry.put());
  if( FAILED(hr) ) throw L"error";

  return pathGeometry;
}

auto CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform) -> winrt::com_ptr<ID2D1TransformedGeometry>
{
  winrt::com_ptr<ID2D1TransformedGeometry> transformedGeometry;
  HRESULT hr = d2dFactory->CreateTransformedGeometry(geometry, transform, transformedGeometry.put());
  if( FAILED(hr) ) throw std::exception();
  return transformedGeometry;
}

auto CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color) -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  HRESULT hr = renderTarget->CreateSolidColorBrush(color, brush.put());
  if( FAILED(hr) ) throw(L"error");
  return brush;
}

auto CreateScreenRenderTextFormat(IDWriteFactory* writeFactory, LPCWSTR fontFamily, DWRITE_FONT_WEIGHT fontWeight, 
  DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize) -> winrt::com_ptr<IDWriteTextFormat>
{
  winrt::com_ptr<IDWriteTextFormat> textFormat;
  HRESULT hr = writeFactory->CreateTextFormat(fontFamily, NULL, fontWeight, fontStyle, fontStretch, fontSize, L"", textFormat.put());
  if( FAILED(hr) ) throw L"error";
  return textFormat;
}

auto CreateDirectSound(HWND window) -> winrt::com_ptr<IDirectSound8>
{
  winrt::com_ptr<IDirectSound8> directSound;

  HRESULT hr = DirectSoundCreate8(NULL, directSound.put(), NULL);

  if( SUCCEEDED(hr) && directSound )
  {
    hr = directSound->SetCooperativeLevel(window, DSSCL_PRIORITY);
    if( FAILED(hr) ) throw std::exception();
  }

  return directSound;
}

auto CreatePrimarySoundBuffer(IDirectSound8* directSound) -> winrt::com_ptr<IDirectSoundBuffer>
{
  winrt::com_ptr<IDirectSoundBuffer> primaryBuffer;
  
  if( directSound )
  {
    DSBUFFERDESC bufferDesc;
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = NULL;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    directSound->CreateSoundBuffer(&bufferDesc, primaryBuffer.put(), NULL);

    if( primaryBuffer )
    {
      WAVEFORMATEX waveFormat;
      waveFormat.wFormatTag = WAVE_FORMAT_PCM;
      waveFormat.nSamplesPerSec = 44100;
      waveFormat.wBitsPerSample = 16;
      waveFormat.nChannels = 2;
      waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
      waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
      waveFormat.cbSize = 0;

      primaryBuffer->SetFormat(&waveFormat);
    }
  }
  
  return primaryBuffer;
}

auto CreateDirectInput(HINSTANCE instance) -> winrt::com_ptr<IDirectInput8>
{
  winrt::com_ptr<IDirectInput8> directInput;
  DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);
  return directInput;
}

auto CreateKeyboard(IDirectInput8* directInput, HWND window) -> winrt::com_ptr<IDirectInputDevice8>
{
  winrt::com_ptr<IDirectInputDevice8> keyboard;
  directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);

  if( keyboard )
  {
    keyboard->SetDataFormat(&c_dfDIKeyboard);
    keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    keyboard->Acquire();
  }

  return keyboard;
}
