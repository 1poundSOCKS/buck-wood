#include "pch.h"
#include "play_screen_state.h"
#include "sound_data.h"

void PlaySoundEffects(const play_screen_state& screenState)
{
  const auto soundBuffers = global_sound_buffer_selector { screenState.soundData.soundBuffers };

//  if( screenState.mode == play_screen_state::playing )
  if( screenState.m_levelState->GetState() == level_state::playing )
  {
    screenState.PlaySoundEffects(soundBuffers);
  }
  else
  {
    StopSoundBufferPlay(soundBuffers[menu_theme]);
    StopSoundBufferPlay(soundBuffers[thrust]);
  }
}
