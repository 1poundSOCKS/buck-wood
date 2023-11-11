#include "pch.h"
#include "game_settings.h"

game_settings* game_settings::m_instance = nullptr;

auto game_settings::Load() -> void
{
  m_effectsVolume = 6;
  m_musicVolume = 7;
}

auto game_settings::Save() -> void
{

}
