#include "sound_buffer.h"

sound_buffer::sound_buffer(const winrt::com_ptr<IDirectSound8>& directSound, const wav_file_data& data)
{
  WAVEFORMATEX waveFormat;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 8;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = 1;
	waveFormat.nAvgBytesPerSec = 44100;
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

  hr = tmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, buffer.put_void());
  if( FAILED(hr) ) throw L"error";
}
