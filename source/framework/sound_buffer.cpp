#include "pch.h"
#include "sound_buffer.h"

winrt::com_ptr<IDirectSoundBuffer8> CreateSoundBuffer(IDirectSound8* directSound, const wav_file_data& data)
{
  winrt::com_ptr<IDirectSoundBuffer8> soundBuffer;

  WAVEFORMATEX waveFormat;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = data.dataFormat.sampleRate;
	waveFormat.wBitsPerSample = data.dataFormat.bitsPerSample;
	waveFormat.nChannels = data.dataFormat.numChannels;
	waveFormat.nBlockAlign = data.dataFormat.blockAlign;
	waveFormat.nAvgBytesPerSec = data.dataFormat.bytesPerSecond;
	waveFormat.cbSize = 0;

  DSBUFFERDESC bufferDesc;
  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = 0;
	bufferDesc.dwBufferBytes = GetWavFileDataSize(data);
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

  winrt::com_ptr<IDirectSoundBuffer> tmpBuffer;
  HRESULT hr = directSound->CreateSoundBuffer(&bufferDesc, tmpBuffer.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = tmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, soundBuffer.put_void());
  if( FAILED(hr) ) throw L"error";

  LPVOID bufferPtr;
  DWORD bufferSize;
  hr = soundBuffer->Lock(0, data.data->size, &bufferPtr, &bufferSize, NULL, 0, 0);
  if( FAILED(hr) ) throw L"error";

  memcpy(bufferPtr, data.data->data.get(), bufferSize);
  hr = soundBuffer->Unlock(bufferPtr, bufferSize, NULL, 0);
  if( FAILED(hr) ) throw L"error";

  return soundBuffer;
}
