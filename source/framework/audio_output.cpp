#include "pch.h"
#include "audio_output.h"
#include "sound.h"

audio_output* audio_output::m_instance = nullptr;

auto audio_output::create(HWND wnd) -> void
{
  destroy();
  m_instance = new audio_output { wnd };
}

auto audio_output::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

audio_output::audio_output(HWND wnd)
{
  if( m_directSound = CreateDirectSound(wnd) )
  {
    m_primarySoundBuffer = CreatePrimarySoundBuffer(m_directSound.get());
  }  
}
