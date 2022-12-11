#ifndef _main_menu_screen_
#define _main_menu_screen_

#include "screens/main_menu/main_menu_screen_state.h"
#include "screen_render_data.h"
#include "timers.h"

void UpdateScreenState
(
  main_menu_screen_state& screenState, 
  const screen_input_state& inputState 
);

void RenderFrame
(
  ID2D1RenderTarget* renderTarget, 
  const bespoke_render_data& renderData, 
  const main_menu_screen_state& screenState
);

void PlaySoundEffects
(
  const main_menu_screen_state& screenState, 
  const bespoke_sound_data& soundData
);

bool ContinueRunning(const main_menu_screen_state& screenState);

#endif
