#include "pch.h"
#include "play_screen_state.h"
#include "sound_buffer_selector.h"

void PlaySoundEffects(const play_screen_state& screenState, global_sound_buffer_selector soundBuffers)
{
  auto& levelState = *screenState.levelState;

  if( levelState.playerShot ) PlaySoundEffect(soundBuffers[shoot]);
  if( levelState.targetShot ) PlaySoundEffect(soundBuffers[target_activated]);
  // if( levelState.player.thrusterOn && screenState.state == play_screen_state::state_playing ) sounds.thrust.PlayOnLoop();
  // else sounds.thrust.Stop();
}
