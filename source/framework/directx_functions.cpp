#include "pch.h"
#include "directx_functions.h"
#include "com_logger.h"

auto CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> d3d_device_and_swap_chain
{
  d3d_device_and_swap_chain deviceAndSwapChain;
  auto& [d3dDevice, swapChain] = deviceAndSwapChain;
  
  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
    featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc, swapChain.put(), d3dDevice.put(), NULL, NULL);
  
  com_logger::fatal(log::type::info, hr, "D3D11CreateDeviceAndSwapChain");

  return deviceAndSwapChain;
}

auto GetDXGIDevice(ID3D11Device* d3dDevice) -> winrt::com_ptr<IDXGIDevice>
{
  winrt::com_ptr<IDXGIDevice> dxgiDevice;
  HRESULT hr = d3dDevice->QueryInterface(dxgiDevice.put());
  com_logger::fatal(log::type::info, hr, "[ID3D11Device] QueryInterface for IDXGIDevice");
  return dxgiDevice;
}

auto CreateDWriteFactory() -> winrt::com_ptr<IDWriteFactory>
{
  winrt::com_ptr<IDWriteFactory> dwriteFactory;
  HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(dwriteFactory),reinterpret_cast<IUnknown**>(dwriteFactory.put()));
  com_logger::fatal(log::type::info, hr, "DWriteCreateFactory");
  return dwriteFactory;
}

auto CreateScreenRenderTextFormat(IDWriteFactory* writeFactory, LPCWSTR fontFamily, DWRITE_FONT_WEIGHT fontWeight, 
  DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize) -> winrt::com_ptr<IDWriteTextFormat>
{
  winrt::com_ptr<IDWriteTextFormat> textFormat;
  HRESULT hr = writeFactory->CreateTextFormat(fontFamily, NULL, fontWeight, fontStyle, fontStretch, fontSize, L"", textFormat.put());
  com_logger::fatal(log::type::debug, hr, "[DWriteFactory] CreateTextFormat");
  return textFormat;
}

auto CreateDirectSound(HWND window) -> winrt::com_ptr<IDirectSound8>
{
  winrt::com_ptr<IDirectSound8> directSound;

  HRESULT hr = DirectSoundCreate8(&DSDEVID_DefaultPlayback, directSound.put(), NULL);
  com_logger::write(log::type::info, hr, "DirectSoundCreate8");

  if( SUCCEEDED(hr) && directSound )
  {
    hr = directSound->SetCooperativeLevel(window, DSSCL_PRIORITY);
    com_logger::write(log::type::info, hr, "[DirectSoundCreate8] SetCooperativeLevel");
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

    HRESULT hr = directSound->CreateSoundBuffer(&bufferDesc, primaryBuffer.put(), NULL);
    com_logger::write(log::type::info, hr, "[IDirectSound8] CreateSoundBuffer (PRIMARY)");

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

      hr = primaryBuffer->SetFormat(&waveFormat);
      com_logger::write(log::type::info, hr, "[IDirectSoundBuffer] SetFormat");
    }
  }
  
  return primaryBuffer;
}

auto CreateDirectInput(HINSTANCE instance) -> winrt::com_ptr<IDirectInput8>
{
  winrt::com_ptr<IDirectInput8> directInput;
  HRESULT hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);
  com_logger::fatal(log::type::info, hr, "DirectInput8Create");
  return directInput;
}

auto CreateKeyboard(IDirectInput8* directInput, HWND window) -> winrt::com_ptr<IDirectInputDevice8>
{
  winrt::com_ptr<IDirectInputDevice8> keyboard;
  HRESULT hr = directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);
  com_logger::fatal(log::type::info, hr, "[IDirectInput8] CreateDevice (KEYBOARD)");

  if( keyboard )
  {
    hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
    com_logger::fatal(log::type::info, hr, "[IDirectInputDevice8] SetDataFormat (KEYBOARD)");
    hr = keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    com_logger::fatal(log::type::info, hr, "[IDirectInputDevice8] SetCooperativeLevel (KEYBOARD)");
    hr = keyboard->Acquire();
    com_logger::write(log::type::info, hr, "[IDirectInputDevice8] Acquire (KEYBOARD)");
  }

  return keyboard;
}
