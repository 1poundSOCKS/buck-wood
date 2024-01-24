#include "pch.h"
#include "sound_buffer_player.h"

sound_buffer_player::sound_buffer_player(const sound_buffer& soundBuffer) : m_soundBuffer { soundBuffer }
{
}

sound_buffer_player::~sound_buffer_player()
{
  m_soundBuffer.Stop();
  m_soundBuffer.Reset();
}

void sound_buffer_player::Play() const
{
  m_soundBuffer.Reset();
  m_soundBuffer.Play(false);
}

void sound_buffer_player::PlayOnLoop() const
{
  m_soundBuffer.Reset();
  m_soundBuffer.Play(true);
}
