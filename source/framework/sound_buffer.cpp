#include "pch.h"
#include "sound_buffer.h"

auto sound_buffer::Create(IDirectSound8* directSound, const wav_file_data& data) -> void
{
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
	bufferDesc.dwBufferBytes = data.GetSize();
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

  winrt::com_ptr<IDirectSoundBuffer> tmpBuffer;

  if( directSound )
  {
    directSound->CreateSoundBuffer(&bufferDesc, tmpBuffer.put(), NULL);
  }

  if( tmpBuffer )
  {
    tmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, m_buffer.put_void());
  }

  if( m_buffer )
  {
    LPVOID bufferPtr;
    DWORD bufferSize;
    
    if( SUCCEEDED(m_buffer->Lock(0, data.data->size, &bufferPtr, &bufferSize, NULL, 0, 0)) )
    {
      memcpy(bufferPtr, data.data->data.get(), bufferSize);
      m_buffer->Unlock(bufferPtr, bufferSize, NULL, 0);
    }
  }
}

auto sound_buffer::Play(bool loop) const -> void
{
  DWORD bufferStatus = S_OK;

  if( m_buffer && SUCCEEDED(m_buffer->GetStatus(&bufferStatus)) && !(bufferStatus & DSBSTATUS_PLAYING) )
  {
    m_buffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
  }
}

auto sound_buffer::Stop() const -> void
{
  DWORD bufferStatus = S_OK;

  if( m_buffer && SUCCEEDED(m_buffer->GetStatus(&bufferStatus)) && bufferStatus & DSBSTATUS_PLAYING )
  {
    m_buffer->Stop();
  }
}

auto sound_buffer::Reset() const -> void
{
  if( m_buffer )
  {
    m_buffer->SetCurrentPosition(0);
  }
}

auto sound_buffer::SetVolume(float value) const -> void
{
  if( m_buffer )
  {
    auto volumeRange = float { DSBVOLUME_MAX - DSBVOLUME_MIN };
    auto volume = static_cast<int>(DSBVOLUME_MIN + volumeRange * value);
    m_buffer->SetVolume(volume);
  }
}
