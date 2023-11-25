#include "pch.h"
#include "game_settings.h"
#include "command_line.h"

game_settings* game_settings::m_instance = nullptr;

auto game_settings::Load() -> void
{
  m_effectsVolume = 6;
  m_musicVolume = 7;

  if( command_line::contains(L"-u") )
  {
    m_framerate = std::nullopt;
  }
}

auto game_settings::Save() -> void
{
}
