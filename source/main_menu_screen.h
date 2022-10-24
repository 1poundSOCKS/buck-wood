#ifndef _main_menu_screen_
#define _main_menu_screen_

#define UNICODE

#include "type_defs.h"
#include "system_timer.h"
#include "d2d_frame.h"
#include "control_state.h"
#include "sound_buffers.h"

struct game_state
{
  game_state();

  bool starting = true;
  bool running = true;
  bool startPlay = false;
  bool quit = false;
};

using game_state_ptr = std::unique_ptr<game_state>;

void RenderFrame(const d2d_frame& frame, game_state& gameState);
void UpdateState(game_state& gameState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const game_state& gameState);

#endif
