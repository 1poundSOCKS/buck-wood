#include "pch.h"
#include "play_screen_state.h"
#include "screen_sound_data.h"

void PlaySoundEffects(const play_screen_state& screenState, const bespoke_sound_data& soundData)
{
  const auto soundBuffers = global_sound_buffer_selector { soundData.soundBuffers };
  auto& levelState = *screenState.levelState;

  if( screenState.state == play_screen_state::state_playing )
  {
    PlaySoundBuffer(soundBuffers[menu_theme], true);

    if( levelState.player.thrusterOn )
      PlaySoundBuffer(soundBuffers[thrust], true);
    else
      StopSoundBufferPlay(soundBuffers[thrust]);

    if( levelState.playerShot )
    {
      ResetSoundBuffer(soundBuffers[shoot]);
      PlaySoundBuffer(soundBuffers[shoot]);
    }

    if( levelState.targetShot )
    {
      ResetSoundBuffer(soundBuffers[shoot]);
      PlaySoundBuffer(soundBuffers[target_activated]);
    }
  }
  else
  {
    StopSoundBufferPlay(soundBuffers[menu_theme]);
    StopSoundBufferPlay(soundBuffers[thrust]);
  }
}
