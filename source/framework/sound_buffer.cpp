#include "pch.h"
#include "sound_buffer.h"

winrt::com_ptr<IDirectSoundBuffer8> LoadSoundBuffer(IDirectSound8* directSound, const std::wstring& path, const std::wstring& file)
{
  std::filesystem::path filename = path;
  filename /= file;
  wav_file_data_ptr soundData = std::make_unique<wav_file_data>(filename.c_str());
  return CreateSoundBuffer(directSound, *soundData);
}

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
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = GetWavFileDataSize(data);
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

  winrt::com_ptr<IDirectSoundBuffer> tmpBuffer;

  if( directSound )
  {
    HRESULT hr = directSound->CreateSoundBuffer(&bufferDesc, tmpBuffer.put(), NULL);
    if( FAILED(hr) ) throw std::exception();

    hr = tmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, soundBuffer.put_void());
    if( FAILED(hr) ) throw std::exception();

    LPVOID bufferPtr;
    DWORD bufferSize;
    hr = soundBuffer->Lock(0, data.data->size, &bufferPtr, &bufferSize, NULL, 0, 0);
    if( FAILED(hr) ) throw std::exception();

    memcpy(bufferPtr, data.data->data.get(), bufferSize);
    hr = soundBuffer->Unlock(bufferPtr, bufferSize, NULL, 0);
    if( FAILED(hr) ) throw std::exception();
  }

  return soundBuffer;
}

void PlaySoundBuffer(IDirectSoundBuffer8* soundBuffer, bool loop)
{
  if( soundBuffer )
  {
    DWORD bufferStatus = S_OK;

    if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
      soundBuffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
  }
}

void StopSoundBufferPlay(IDirectSoundBuffer8* soundBuffer)
{
  if( soundBuffer )
  {
    DWORD bufferStatus = S_OK;

    if( SUCCEEDED(soundBuffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
      soundBuffer->Stop();
  }
}

void ResetSoundBuffer(IDirectSoundBuffer8* soundBuffer)
{
  if( soundBuffer )
  {
    soundBuffer->SetCurrentPosition(0);
  }
}
