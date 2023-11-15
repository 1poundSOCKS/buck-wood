#include "pch.h"
#include "audio_output.h"
#include "sound.h"

audio_output* audio_output::m_instance = nullptr;

auto audio_output::create(HWND wnd) -> void
{
  m_instance = new audio_output { wnd };
}

audio_output::audio_output(HWND wnd)
{
  if( m_directSound = CreateDirectSound(wnd) )
  {
    m_primarySoundBuffer = CreatePrimarySoundBuffer(m_directSound.get());
  }  
}
