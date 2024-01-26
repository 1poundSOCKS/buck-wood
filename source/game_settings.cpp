#include "pch.h"
#include "game_settings.h"
#include "command_line.h"

game_settings* game_settings::m_instance = nullptr;

auto game_settings::Load() -> void
{
  if( command_line::contains(L"-u") )
  {
    m_framerate = std::nullopt;
  }
}

auto game_settings::Save() -> void
{
}
