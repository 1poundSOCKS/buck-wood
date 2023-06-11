#include "pch.h"

winrt::com_ptr<IDirectSound8> CreateDirectSound(HWND window)
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

winrt::com_ptr<IDirectSoundBuffer> CreatePrimarySoundBuffer(IDirectSound8* directSound)
{
  DSBUFFERDESC bufferDesc;
  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

  winrt::com_ptr<IDirectSoundBuffer> primaryBuffer;
  HRESULT hr = directSound->CreateSoundBuffer(&bufferDesc, primaryBuffer.put(), NULL);
  if( FAILED(hr) ) throw std::exception();

  WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	hr = primaryBuffer->SetFormat(&waveFormat);
  if( FAILED(hr) ) throw std::exception();
  
  return primaryBuffer;
}
