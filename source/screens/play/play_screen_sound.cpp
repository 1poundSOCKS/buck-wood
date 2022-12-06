#include "pch.h"
#include "play_screen_state.h"
#include "screen_sound_data.h"

void PlaySoundEffects(const play_screen_state& screenState, const bespoke_sound_data& soundData)
{
  const auto soundBuffers = global_sound_buffer_selector { soundData.soundBuffers };
  auto& levelState = *screenState.levelState;

  if( levelState.playerShot ) PlaySoundBuffer(soundBuffers[shoot]);
  if( levelState.targetShot ) PlaySoundBuffer(soundBuffers[target_activated]);
  if( levelState.player.thrusterOn && screenState.state == play_screen_state::state_playing ) PlaySoundBuffer(soundBuffers[thrust], true);
  else StopSoundBufferPlay(soundBuffers[thrust]);
}
